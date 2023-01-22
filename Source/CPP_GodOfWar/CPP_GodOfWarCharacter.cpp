// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPP_GodOfWarCharacter.h"

#include "CPP_AxeProjectile.h"
#include "CPP_EnemyMonster.h"
#include "CPP_EnemySpawner.h"
#include "CPP_MainHUD_W.h"
#include "CPP_PickableBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Health_Rage_W.h"
#include "InteractInterface.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"


//////////////////////////////////////////////////////////////////////////
// ACPP_GodOfWarCharacter
//ConstructionScript
ACPP_GodOfWarCharacter::ACPP_GodOfWarCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 460.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh(), "spine_03");
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	RageHandL = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RageHandL"));
	RageHandL->SetupAttachment(GetMesh(), "hand_l");

	RageHandR = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RageHandR"));
	RageHandR->SetupAttachment(GetMesh(), "hand_r");

	WeaponSocketSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSceneComponent"));
	WeaponSocketSceneComponent->SetupAttachment(GetMesh(), "WeaponSocket");

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponHand"));
	WeaponMesh->SetupAttachment(GetMesh(), "WeaponHandSocket");

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComp->SetupAttachment(GetRootComponent());

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACPP_GodOfWarCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsDead)
	{
		DeathControl();
		HealthRageUpdate();
		WallTrace();
	}
}

/*BeginPlay*/

void ACPP_GodOfWarCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	if (!IsInMainMenu)
	{
		if (wCrossHair)
		{
			CrossHair_W = CreateWidget<UUserWidget>(this->GetGameInstance(), wCrossHair);
		}
		if (wPauseMenu)
		{
			Pause_Widget = CreateWidget<UUserWidget>(this->GetGameInstance(), wPauseMenu);
		}
		if (wMainHUD)
		{
			MainHUD = CreateWidget<UUserWidget>(this->GetGameInstance(), wMainHUD);
			MainHUD_Ref = Cast<UCPP_MainHUD_W>(MainHUD);
			MainHUD->AddToViewport(9);
			if (MainHUD_Ref)
			{
				HealthRageBar_W = MainHUD_Ref->HealthRageBarW;
			}
		}
	}
	//UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake();
}

/*BeginPlay_End*/

//////////////////////////////////////////////////////////////////////////
// Input

void ACPP_GodOfWarCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ACPP_GodOfWarCharacter::SetDefaultMovement);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::Look);

		//Attacking
		EnhancedInputComponent->BindAction(Attack, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::AttackInputEvent);

		//Kick
		EnhancedInputComponent->BindAction(KickInput, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::KickEvent);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACPP_GodOfWarCharacter::Walk);

		//Aiming
		EnhancedInputComponent->BindAction(Aim, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::AimAction);
		EnhancedInputComponent->BindAction(Aim, ETriggerEvent::Completed, this, &ACPP_GodOfWarCharacter::StopAim);

		//ReturnAxe
		EnhancedInputComponent->BindAction(ReturnInput, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::ReturnAxe);

		//EquipAxe
		EnhancedInputComponent->BindAction(EquipInput, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::EquipUnequip);

		EnhancedInputComponent->BindAction(RageActiveInput, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::StartRage);

		EnhancedInputComponent->BindAction(RollInputAction, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::RollAnimationStart);

		EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::InteractEvent);

		EnhancedInputComponent->BindAction(PauseMenuInputAction, ETriggerEvent::Triggered, this, &ACPP_GodOfWarCharacter::FPauseGame);
	}

}

//Input_End

/*Movement_Input_Functions*/
void ACPP_GodOfWarCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACPP_GodOfWarCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

/*Movement_Functions_End*/

/*Melee_Attack_Functions*/

 //Input girdisiyle tetiklenen atak fonksiyonu
void ACPP_GodOfWarCharacter::AttackInputEvent()
{
	if (!IsAiming && !IsDead)
	{
		if (InHand && IsEquipped)
		{
			if (CanAttack)
			{ //Atak animasyonu array i icin tutulan sayiyi kontrol edip animasyonu oynatma
				if (AttackCount > 3)
				{
					AttackCount = 0;
				}
				GetMesh()->GetAnimInstance()->Montage_Play(AttackAnim[AttackCount]);
				
				if (!IsBlockingWall)
				{
					FVector CharacterTargetLocation = ArrowComp->GetComponentLocation();
					FRotator CharacterTargetRotation = GetCapsuleComponent()->GetComponentRotation();
					FLatentActionInfo LatentInfo;
					LatentInfo.CallbackTarget = this;
					TEnumAsByte<EMoveComponentAction::Type> ActionMode = EMoveComponentAction::Type::Move;
					UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), CharacterTargetLocation, CharacterTargetRotation,
						false, true, 0.45f, false, ActionMode, LatentInfo);
				}
				CanAttack = false;
				AttackCount++;
			}
		}
		else
		{
			if (!InHand)
			{
				if (EnemyRef && CanAttack)
				{
					CanAttack = false;

					FTransform CapsuleTransform = GetCapsuleComponent()->GetComponentTransform();
					FTransform EnemyArrowTransform = EnemyRef->GetCharacterArrow()->GetComponentTransform();

					FVector RangeValue = CapsuleTransform.GetLocation() - EnemyArrowTransform.GetLocation();

					float RangeValue_x = RangeValue.X;
					float RangeValue_y = RangeValue.Y;
					/*Karakter menzilde degil ise karakteri dusmanin menziline tasir*/

					bool InAttackRange = UKismetMathLibrary::InRange_FloatFloat(RangeValue_x, -400.f, 400.f, true, true) && 
						UKismetMathLibrary::InRange_FloatFloat(RangeValue_y, -350.f, 350.f, true, true);
					IsClose = UKismetMathLibrary::InRange_FloatFloat(RangeValue_x, -110.f, 110.f, true, true) && 
						UKismetMathLibrary::InRange_FloatFloat(RangeValue_y, -110.f, 110.f, true, true);
					if (InAttackRange)
					{
						if (!IsClose)
						{
							FVector CharacterMoveLoc = EnemyRef->GetCharacterArrow()->GetComponentLocation();
							FRotator CharacterMoveRot = EnemyRef->GetCharacterArrow()->GetComponentRotation();
							FLatentActionInfo LatentInfo;
							LatentInfo.CallbackTarget = this;
							TEnumAsByte<EMoveComponentAction::Type> ActionMode = EMoveComponentAction::Type::Move;
							//GetMesh()->GetAnimInstance()->Montage_Play(HandMoveIdle);
							UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), CharacterMoveLoc, CharacterMoveRot,
							                                      false, true, 0.45f, false, ActionMode, LatentInfo);
							CanAttack = true;
						}
						else
						{
							CanAttack = true;
						}
					}
					else
					{
						CanAttack = true;
					}
				}
				if (CanAttack)
				{ //Siradaki fist animasyonunu seçme ve oynatma blogu
					if (HandAttackCount > 2)
					{
						HandAttackCount = 0;
					}
					GetMesh()->GetAnimInstance()->Montage_Play(HandAttackAnim[HandAttackCount]);
					CanAttack = false;
					if (IsClose)
					{
						if (!IsBlockingWall)
						{
							FVector CharacterTargetLoc = ArrowComp->GetComponentLocation();
							FRotator CharacterTargetRot = GetCapsuleComponent()->GetComponentRotation();
							FLatentActionInfo LatentInfo;
							LatentInfo.CallbackTarget = this;
							TEnumAsByte<EMoveComponentAction::Type> ActionMode = EMoveComponentAction::Type::Move;
							UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), CharacterTargetLoc, CharacterTargetRot,
								false, true, 0.45f, false, ActionMode, LatentInfo);
						}
					}
					HandAttackCount++;
				}
			}
		}
	}
	else
	{
		if (InHand && !IsDead)
		{ //Baltayi firlatmak için animasyon ve fonksiyon tetikleme
			InHand = false;
			GetCharacterMovement()->DisableMovement();
			GetMesh()->GetAnimInstance()->Montage_Play(AxeThrowAnim);
			UGameplayStatics::PlaySound2D(GetWorld(), AxeThrowSound);
			GetWorldTimerManager().SetTimer(TH_Dissapear, this, &ACPP_GodOfWarCharacter::DissappearingAxe, 0.40f, false);
		}
	}
	
}

//AnimNotify ile tetiklenen atak fonksiyonu
void ACPP_GodOfWarCharacter::TriggerAttackEvent()
{ 
	GetWorldTimerManager().SetTimer(TH_Attack, this, &ACPP_GodOfWarCharacter::TriggerAttack, TraceTime, true);
}

void ACPP_GodOfWarCharacter::GetDamageFromEnemy()
{
	GetMesh()->GetAnimInstance()->Montage_Play(HitReaction);
	UGameplayStatics::PlaySound2D(GetWorld(), HitReactionSound);
	GetWorldTimerManager().ClearTimer(TH_Attack);
	CanAttack = true;
	CanDamage = true;
	CanKick = true;
	IsDodging = false;
	GetWorldTimerManager().ClearTimer(TH_Kick);
}

//TriggerAttackEvent ten tetiklenen zaman ayarli atak trace cizdirme fonksiyonu
void ACPP_GodOfWarCharacter::TriggerAttack()
{
	if (IsEquipped && InHand)
	{ //Balta ile saldirirken trace cekme
		FHitResult HitResult;
		FVector Start = WeaponMesh->GetSocketLocation("start");
		FVector End = WeaponMesh->GetSocketLocation("end");
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, QueryParams);
		if (HitResult.GetActor())
		{
			EnemyRef = Cast<ACPP_EnemyMonster>(HitResult.GetActor());
			if (EnemyRef && CanDamage)
			{
				//Enemy take damage
				if (AttackCount == 3)
				{
					GetWorldTimerManager().SetTimer(TH_JumpEnemy, this, &ACPP_GodOfWarCharacter::JumpEnemy, 0.01f, false);
				}
				EnemyRef->GetAnyDamage(AxeDamage);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitEnemySound, EnemyRef->GetActorLocation());
				UGameplayStatics::PlaySoundAtLocation(EnemyRef, EnemyRef->EHitReactionSound, EnemyRef->GetMesh()->GetSocketLocation("Head"), EnemyRef->GetMesh()->GetSocketRotation("Head"));
				FVector EnemyTargetLocation = EnemyRef->GetDamageArrow()->GetComponentLocation();
				FRotator EnemyTargetRotation = EnemyRef->GetCapsuleComponent()->GetComponentRotation();
				FLatentActionInfo LatentInfo;
				LatentInfo.CallbackTarget = EnemyRef;
				TEnumAsByte<EMoveComponentAction::Type> ActionMode = EMoveComponentAction::Type::Move;
				UKismetSystemLibrary::MoveComponentTo(EnemyRef->GetCapsuleComponent(), EnemyTargetLocation, EnemyTargetRotation,
					false, true, 0.45f, false, ActionMode, LatentInfo);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticle, HitResult.ImpactPoint, HitResult.Normal.Rotation());
				EnemyRef->GetMesh()->GetAnimInstance()->Montage_Play(EnemyRef->HitReactionEnemy);
				EnemyRef->Stun();
				CanDamage = false;
			}
		}
	}
	else
	{ //Ciplak el ile saldirirken trace cekme
		if (HandIsRight)
		{ //Sag el icin trace
			FHitResult HitResult;
			FVector StartPos = GetMesh()->GetSocketLocation("hand_start");
			FVector EndPos = GetMesh()->GetSocketLocation("hand_end");
			FCollisionQueryParams Params;
			FQuat Rot;
			Params.AddIgnoredActor(this);
			GetWorld()->SweepSingleByChannel(HitResult, StartPos, EndPos, Rot, ECC_Visibility, FCollisionShape::MakeSphere(30.f), Params);
			if (HitResult.GetActor())
			{
				EnemyRef = Cast<ACPP_EnemyMonster>(HitResult.GetActor());
				if (EnemyRef && CanDamage)
				{
					if (EnemyRef->IsFreeze)
					{
						EnemyRef->UnFreeze();
					}
					EnemyRef->GetAnyDamage(HandDamage);
					FVector EnemyTargetLocation = EnemyRef->GetDamageArrow()->GetComponentLocation();
					FRotator EnemyTargetRotation = EnemyRef->GetCapsuleComponent()->GetComponentRotation();
					FLatentActionInfo LatentInfo;
					LatentInfo.CallbackTarget = EnemyRef;
					TEnumAsByte<EMoveComponentAction::Type> ActionMode = EMoveComponentAction::Type::Move;
					UKismetSystemLibrary::MoveComponentTo(EnemyRef->GetCapsuleComponent(), EnemyTargetLocation, EnemyTargetRotation,
						false, true, 0.45f, false, ActionMode, LatentInfo);
					UGameplayStatics::PlaySound2D(GetWorld(), PunchHitSound);
					UGameplayStatics::PlaySoundAtLocation(EnemyRef, EnemyRef->EHitReactionSound, GetMesh()->GetSocketLocation("Head"), GetMesh()->GetSocketRotation("Head"));
					if (IsInRage)
					{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireHand, HitResult.ImpactPoint, HitResult.Normal.Rotation());
					}
					else
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticle, HitResult.ImpactPoint, HitResult.Normal.Rotation());
					}
					EnemyRef->GetMesh()->GetAnimInstance()->Montage_Play(EnemyRef->HitReactionEnemy);
					EnemyRef->Stun();
					CanDamage = false;
				}
			}
			else
			{
				UGameplayStatics::PlaySound2D(GetWorld(), PunchMissSound);
			}
		}
		else
		{ //Sol el icin trace
			FHitResult HitResult;
			FVector StartPos = GetMesh()->GetSocketLocation("hand_start_l");
			FVector EndPos = GetMesh()->GetSocketLocation("hand_end_l");
			FCollisionQueryParams Params;
			FQuat Rot;
			Params.AddIgnoredActor(this);
			GetWorld()->SweepSingleByChannel(HitResult, StartPos, EndPos, Rot, ECC_Visibility, FCollisionShape::MakeSphere(30.f), Params);
			if (HitResult.GetActor())
			{
				EnemyRef = Cast<ACPP_EnemyMonster>(HitResult.GetActor());
				if (EnemyRef && CanDamage)
				{
					if (EnemyRef->IsFreeze)
					{
						EnemyRef->UnFreeze();
					}
					EnemyRef->GetAnyDamage(HandDamage);
					FVector EnemyTargetLocation = EnemyRef->GetDamageArrow()->GetComponentLocation();
					FRotator EnemyTargetRotation = EnemyRef->GetCapsuleComponent()->GetComponentRotation();
					FLatentActionInfo LatentInfo;
					LatentInfo.CallbackTarget = EnemyRef;
					TEnumAsByte<EMoveComponentAction::Type> ActionMode = EMoveComponentAction::Type::Move;
					UKismetSystemLibrary::MoveComponentTo(EnemyRef->GetCapsuleComponent(), EnemyTargetLocation, EnemyTargetRotation,
						false, true, 0.45f, false, ActionMode, LatentInfo);
					UGameplayStatics::PlaySoundAtLocation(EnemyRef, EnemyRef->EHitReactionSound, GetMesh()->GetSocketLocation("Head"), GetMesh()->GetSocketRotation("Head"));
					if (IsInRage)
					{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireHand, HitResult.ImpactPoint, HitResult.Normal.Rotation());
						UGameplayStatics::PlaySound2D(GetWorld(), RagePunchSound);
					}
					else
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticle, HitResult.ImpactPoint, HitResult.Normal.Rotation());
						UGameplayStatics::PlaySound2D(GetWorld(), PunchHitSound);
					}
					EnemyRef->GetMesh()->GetAnimInstance()->Montage_Play(EnemyRef->HitReactionEnemy);
					EnemyRef->Stun();
					CanDamage = false;
				}
			}
			else
			{
				UGameplayStatics::PlaySound2D(GetWorld(), PunchMissSound);
			}
		}
	}
}

//Anim notify ile tetiklenen atagi durdurma fonksiyonu
void ACPP_GodOfWarCharacter::StopAttackEvent()
{ 
	GetWorldTimerManager().SetTimer(TH_WaitNextAttack, this, &ACPP_GodOfWarCharacter::StoppingAttack, 0.3f, false);
}

void ACPP_GodOfWarCharacter::StoppingAttack()
{
	GetWorldTimerManager().ClearTimer(TH_Attack);
	GetWorldTimerManager().ClearTimer(TH_WaitNextAttack);
	CanAttack = true;
	CanDamage = true;
}

//Tekme atip atamayacagini kontrol edip animasyon oynatma
void ACPP_GodOfWarCharacter::KickEvent()
{
	if (CanKick && !IsDead)
	{
		CanKick = false;
		GetMesh()->GetAnimInstance()->Montage_Play(KickAnim);
	}
}

//AnimNotify ile tetiklenen tekme fonksiyonu
void ACPP_GodOfWarCharacter::KickTriggerEvent()
{
	GetWorldTimerManager().SetTimer(TH_Kick, this, &ACPP_GodOfWarCharacter::StartKickAttack, TraceTime, true);
}

//AnimNotify ile tetiklenen zaman ayarlý tekme trace cizdirme fonksiyonu
void ACPP_GodOfWarCharacter::StartKickAttack()
{
	FHitResult HitResult;
	FVector StartPos = GetMesh()->GetSocketLocation("calf_r");
	FVector EndPos = GetMesh()->GetSocketLocation("ball_r");
	FCollisionQueryParams Params;
	FQuat Rot;
	Params.AddIgnoredActor(this);
	GetWorld()->SweepSingleByChannel(HitResult, StartPos, EndPos, Rot, ECC_Visibility, FCollisionShape::MakeSphere(30.f), Params);
	if (HitResult.GetActor())
	{
		EnemyRef = Cast<ACPP_EnemyMonster>(HitResult.GetActor());
		if (EnemyRef && CanDamage)
		{
			if (EnemyRef->IsFreeze)
			{
				EnemyRef->UnFreeze();
			}
			EnemyRef->KratosRotation = GetCapsuleComponent()->GetRelativeRotation();
			EnemyRef->GetAnyDamage(HandDamage);
			EnemyRef->RagdollFunction(1.97f, false);
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticle, HitResult.ImpactPoint, HitResult.Normal.Rotation());
			CanDamage = false;
		}
	}
}

//AnimNotify ile tetiklenen tekme durdurma fonksiyonu
void ACPP_GodOfWarCharacter::StopKickEvent()
{
	CanKick = true;
	CanDamage = true;
	GetWorldTimerManager().ClearTimer(TH_Kick);
}

void ACPP_GodOfWarCharacter::StartRage()
{
	if (CanStartRage && !IsDead)
	{
		CanStartRage = false;
		IsInRage = true;
		if (IsEquipped)
		{
			IsPressed = false;
			EquipUnequip();
		}
		HandDamage += ActiveRageDamage;
		RageHandL->SetVisibility(true);
		RageHandR->SetVisibility(true);
		Rage -= 1.f;
		UGameplayStatics::PlaySound2D(GetWorld(), StartRageSound);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), StartRageKratosSound, GetMesh()->GetSocketLocation("head"));
		GetWorldTimerManager().SetTimer(TH_RageCalculate, this, &ACPP_GodOfWarCharacter::RageControl, 0.01f, true);
	}
}

void ACPP_GodOfWarCharacter::RageControl()
{
	if (Rage < 0.f)
	{
		StopRage();
		GetWorldTimerManager().ClearTimer(TH_RageCalculate);
	}

	if (!IsInRage)
	{
		StopRage();
		GetWorldTimerManager().ClearTimer(TH_RageCalculate);
	}
	else if (IsInRage)
	{
		Rage -= 0.05f;
	}
	
}

void ACPP_GodOfWarCharacter::StopRage()
{
	RageHandL->SetVisibility(false);
	RageHandR->SetVisibility(false);
	UGameplayStatics::PlaySound2D(GetWorld(), StopRageSound);
	HandDamage -= ActiveRageDamage;
	IsInRage = false;
	GetWorldTimerManager().SetTimer(TH_RegenerateRage, this, &ACPP_GodOfWarCharacter::RegenerateRage, 0.01f, true);
}

void ACPP_GodOfWarCharacter::RegenerateHealth()
{
	Health += 0.01f;
	Health = FMath::Clamp(Health, 0.f, 100.f);
	if (Health >= 45.f)
	{
		IsRegenerateHealth = false;
		GetWorldTimerManager().ClearTimer(TH_RegenerateHealth);
	}
}

void ACPP_GodOfWarCharacter::RegenerateRage()
{
	Rage += 0.01f;
	Rage = FMath::Clamp(Rage, 0.f, 100.f);
	if (Rage == 100.f)
	{
		GetWorldTimerManager().ClearTimer(TH_RegenerateRage);
	}
}

//Aim_Event

void ACPP_GodOfWarCharacter::AimAction()
{
	if (InHand)
	{
		IsAiming = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		if (CrossHair_W)
		{
			if (!CrossHair_W->IsInViewport())
			{
				CrossHair_W->AddToViewport(9);
			}
		}
	}
}

void ACPP_GodOfWarCharacter::StopAim()
{
	IsAiming = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	if (CrossHair_W)
	{
		if (CrossHair_W->IsInViewport())
		{
			CrossHair_W->RemoveFromParent();
		}
	}
}
//Aim_End

//Baltayi firlatirken tam zamanýnda elden cikarmak icin zaman ayarlý fonksiyon
void ACPP_GodOfWarCharacter::DissappearingAxe()
{
	WeaponMesh->SetVisibility(false);
	IsAiming = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	//Baltayi firlattiktan sonra projectile spawn etme
	FVector SpawnLocation = FollowCamera->GetComponentLocation();
	FRotator SpawnRotator = FollowCamera->GetComponentRotation();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(AxeProjectileClass, SpawnLocation, SpawnRotator, Params);
	if (SpawnedActor)
	{
		AxeProjectileRef = Cast<ACPP_AxeProjectile>(SpawnedActor);
		if (AxeProjectileRef)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), AxeProjectileRef->AxeStartSound);
			AxeProjectileRef->IsReturned = false;
		}
	}
	GetWorldTimerManager().ClearTimer(TH_Dissapear);
}

//Firlatilan baltayi geri cagirma input tetikleme
void ACPP_GodOfWarCharacter::ReturnAxe()
{
	if (!InHand && IsEquipped)
	{ //Firlatilan baltayi geri cagirma
		AxeProjectileRef->Return();
	}
}

//Geri gelen projectile class ini destroy edip baltanin visibility'sini acma 
void ACPP_GodOfWarCharacter::ReappearingAxe()
{ 
	WeaponMesh->SetVisibility(true);
	InHand = true;
	IsEquipped = true;
}

//Baltayi alma ve birakma
void ACPP_GodOfWarCharacter::EquipUnequip()
{
	if (!IsPressed)
	{
		if (IsEquipped && InHand)
		{ //Baltayi sirtina asma
			IsPressed = true;
			CanAttack = false;
			GetMesh()->GetAnimInstance()->Montage_Play(UnequipAnim);
			GetWorldTimerManager().SetTimer(TH_AttachTimer, this, &ACPP_GodOfWarCharacter::UnequipFunction, 0.63f, false);
		}
		else if (!IsEquipped && !InHand)
		{ //Baltayi eline alma
			IsPressed = true;
			if (IsInRage)
			{
				IsInRage = false;
				CanStartRage = true;
			}
			CanAttack = false;
			GetMesh()->GetAnimInstance()->Montage_Play(EquipAnim);
			GetWorldTimerManager().SetTimer(TH_AttachTimer, this, &ACPP_GodOfWarCharacter::EquipFunction, 0.63f, false);
		}
	}
}

//Animasyon icin zaman ayarli tetiklenen equip fonksiyonu
void ACPP_GodOfWarCharacter::EquipFunction()
{ 
	WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponHandSocket");
	InHand = true;
	IsEquipped = true;
	CanAttack = true;
	IsPressed = false;
	GetWorldTimerManager().ClearTimer(TH_AttachTimer);
}

//Animasyon icin zaman ayarli tetiklenen unequip fonksiyonu
void ACPP_GodOfWarCharacter::UnequipFunction()
{ 
	WeaponMesh->AttachToComponent(WeaponSocketSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "None");
	WeaponMesh->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
	InHand = false;
	IsEquipped = false;
	CanAttack = true;
	IsPressed = false;
	GetWorldTimerManager().ClearTimer(TH_AttachTimer);
}

//Karakterin hayatta olup olmadigini kontrol eden fonksiyon
void ACPP_GodOfWarCharacter::DeathControl()
{ 
	if (Health <= 0)
	{
		IsDead = true;
		CanAttack = false;
		InHand = false;
		IsEquipped = false;
		GetCharacterMovement()->DisableMovement();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		APlayerController* CharacterController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (CharacterController)
		{
			Restart_Widget = CreateWidget<UUserWidget>(this->GetGameInstance(), wRestartMenu);
			if (Restart_Widget)
			{
				Restart_Widget->AddToViewport(10);
			}
		}

	}
}

void ACPP_GodOfWarCharacter::JumpEnemy()
{
	if (EnemyRef)
	{
		if (EnemyRef->GetHealth() > 30.f)
		{
			EnemyRef->RagdollFunction(2.65f, true);
		}
	}
}

void ACPP_GodOfWarCharacter::HealthRageUpdate()
{
	if (HealthRageBar_W)
	{
		float newHealth = FMath::Clamp(Health / 100.f, -1.f, 1.f);
		float newRage = FMath::Clamp(Rage / 100.f, 0.f, 1.f);
		HealthRageBar_W->UpdateHealthRage(newHealth, newRage);
		if (Rage == 100.f)
		{
			CanStartRage = true;
		}
		if (Health <= 35.f && !IsRegenerateHealth)
		{
			IsRegenerateHealth = true;
			GetWorldTimerManager().SetTimer(TH_RegenerateHealth, this, &ACPP_GodOfWarCharacter::RegenerateHealth, 0.01f, true);
		}
	}
}

//Kosma fonksiyonu
void ACPP_GodOfWarCharacter::StartSprint()
{ 
	GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
}

//Yurume fonksiyonu
void ACPP_GodOfWarCharacter::Walk()
{ 
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

void ACPP_GodOfWarCharacter::SetDefaultMovement()
{
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void ACPP_GodOfWarCharacter::WallTrace()
{
	FHitResult HitResults;
	FVector Start = GetMesh()->GetSocketLocation("pelvis");
	FVector End = ArrowComp->GetForwardVector() * 100.f;
	End = Start + End;
	FCollisionQueryParams Parameters;
	Parameters.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResults, Start, End, ECC_Visibility, Parameters);
	if (HitResults.bBlockingHit)
	{
		if (HitResults.GetActor())
		{
			EnemyRef = Cast<ACPP_EnemyMonster>(HitResults.GetActor());
			if (!EnemyRef)
			{
				IsBlockingWall = true;
			}
			else
			{
				IsBlockingWall = false;
			}
		}
	}
	else
	{
		IsBlockingWall = false;
	}
}

void ACPP_GodOfWarCharacter::InteractEvent()
{
	if (CanInteractObject)
	{
		if (PickableRef)
		{
			IInteractInterface* Interact = Cast<IInteractInterface>(PickableRef);
			if (Interact)
			{
				PickableRef->Interact();
			}
		}
	}
}

void ACPP_GodOfWarCharacter::RollAnimationStart()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		ForwardDodge = MovementVector.Y;
		RightDodge = MovementVector.X;

		if (GetCharacterMovement()->Velocity.Length() > 460)
		{
			if (ForwardDodge != 0.f && !IsDodging)
			{
				if (ForwardDodge > 0.f)
				{
					GetMesh()->GetAnimInstance()->Montage_Play(RollForwardAnim, 1.5f);
					IsDodging = true;
				}
				if (ForwardDodge < 0.f)
				{
					GetMesh()->GetAnimInstance()->Montage_Play(RollBackAnim, 1.5f);
					IsDodging = true;
				}
			}
			if (RightDodge != 0.f && !IsDodging)
			{
				if (RightDodge > 0.f)
				{
					GetMesh()->GetAnimInstance()->Montage_Play(RollLeftAnim, 1.5f);
					IsDodging = true;
				}
				if (RightDodge < 0.f)
				{
					GetMesh()->GetAnimInstance()->Montage_Play(RollRightAnim, 1.5f);
					IsDodging = true;
				}
			}
		}
	}
	
}

void ACPP_GodOfWarCharacter::RollDelay()
{
	FTimerHandle TH_RollDelay;
	GetWorldTimerManager().SetTimer(TH_RollDelay, this, &ACPP_GodOfWarCharacter::RollActive, .5f, false);
}

void ACPP_GodOfWarCharacter::RollActive()
{
	IsDodging = false;
}

void ACPP_GodOfWarCharacter::FPauseGame() 
{
	if (Pause_Widget && !IsPausedGame)
	{
		IsPausedGame = true;
		if (!Pause_Widget->IsInViewport())
		{
			Pause_Widget->AddToViewport(9);
		}
	}
	else
	{
		FUnPauseGame();
	}
}

void ACPP_GodOfWarCharacter::FUnPauseGame()
{
	if (Pause_Widget && IsPausedGame)
	{
		IsPausedGame = false; 
		if (Pause_Widget->IsInViewport())
		{
			APlayerController* ControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (ControllerRef)
			{
				ControllerRef->SetPause(false);
				ControllerRef->SetInputMode(FInputModeGameOnly());
				ControllerRef->SetShowMouseCursor(false);
			}
			Pause_Widget->RemoveFromParent();
		}
	}
}