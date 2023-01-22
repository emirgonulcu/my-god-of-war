// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_EnemyMonster.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "CPP_EnemySpawner.h"
#include "CPP_GodOfWarCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ACPP_EnemyMonster::ACPP_EnemyMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowChr = CreateDefaultSubobject<UArrowComponent>(TEXT("CharacterLocation"));
	ArrowChr->SetupAttachment(GetRootComponent());

	ArrowDmg = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowDamage"));
	ArrowDmg->SetupAttachment(GetRootComponent());

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void ACPP_EnemyMonster::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPP_EnemyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Death();
	if (AIC_Ref)
	{
		if (MoveRandomLocBool)
		{
			MoveToRandomLocation();
			MoveRandomLocBool = false;
			EnemySoundController = UGameplayStatics::SpawnSoundAtLocation(this, EnemySound, GetMesh()->GetSocketLocation("Head"), GetMesh()->GetSocketRotation("Head"));
		}
		//Yapay zeka random dolasma
		if (Investigate)
		{
			InvestigateTrace();
		}
		//Yapay zeka karakteri gorunce
		if (SeeingPlayer && !Investigate)
		{
			SeeingPlayer = false;
			StartChasePlayer();
		}
	}
	else
	{
		//AI controller referansi alma
		AIC_Ref = Cast<AAIController>(GetController());
	}
}

void ACPP_EnemyMonster::InvestigateTrace()
{
	if (Investigate)
	{
		FHitResult Hit;
		FVector Start = GetDamageArrow()->GetComponentLocation();
		FVector End = GetDamageArrow()->GetForwardVector() * 10000.f;
		FCollisionQueryParams Params;
		FQuat Rot;
		Params.AddIgnoredActor(this);
		GetWorld()->SweepSingleByChannel(Hit, Start, End, Rot, ECC_Camera, FCollisionShape::MakeSphere(60.f), Params);
		if (Hit.GetActor())
		{
			CharacterRef = Cast<ACPP_GodOfWarCharacter>(Hit.GetActor());
			if (CharacterRef)
			{
				Investigate = false;
				SeeingPlayer = true;
				CanAttackToPlayer = true;
				AIC_Ref->StopMovement();
				AIC_Ref->ReceiveMoveCompleted.Clear();
			}
		}
	}
}

//Yapay zekaya random location buldurma
void ACPP_EnemyMonster::MoveToRandomLocation()
{ 
	if (AIC_Ref && !IsDead && !HitStun && !IsFreeze)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem)
		{
			FVector StartPos = GetActorLocation();
			FNavLocation EndPos = FNavLocation(StartPos);
			NavSystem->GetRandomReachablePointInRadius(StartPos, 1000.f, EndPos);
			AIC_Ref->MoveToLocation(EndPos.Location);
			AIC_Ref->ReceiveMoveCompleted.AddDynamic(this, &ACPP_EnemyMonster::MoveComplete);
		}
	}
	else
	{
		AIC_Ref->StopMovement();
		AIC_Ref->ReceiveMoveCompleted.Clear();
	}
}

void ACPP_EnemyMonster::MoveComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == 0)
	{
		if (!HitStun)
		{
			AIC_Ref->ReceiveMoveCompleted.Clear();
			GetWorldTimerManager().SetTimer(TH_MoveLoop, this, &ACPP_EnemyMonster::MoveToRandomLocation, 1.4f, false);
		}
	}
}

void ACPP_EnemyMonster::StartChasePlayer()
{
	AIC_Ref->StopMovement();
	AIC_Ref->ReceiveMoveCompleted.Clear();
	GetWorldTimerManager().SetTimer(TH_Tick, this, &ACPP_EnemyMonster::ChasePlayer, 1.35f, true);
}

//Karakteri takip etme
void ACPP_EnemyMonster::ChasePlayer()
{ 
	if (AIC_Ref && !IsDead && !HitStun && !IsFreeze)
	{
		AIC_Ref->MoveToActor(CharacterRef, 100.f, false);
		AIC_Ref->ReceiveMoveCompleted.AddDynamic(this, &ACPP_EnemyMonster::AttackPlayer);
	}
	else
	{
		AIC_Ref->StopMovement();
		AIC_Ref->ReceiveMoveCompleted.Clear();
	}
}

//Yeteri kadar yaklasirsa karaktere saldirma
void ACPP_EnemyMonster::AttackPlayer(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{ 
	if (Result == 0 && !IsFreeze && !IsDead && !HitStun && CanAttackToPlayer)
	{
		AIC_Ref->ReceiveMoveCompleted.Clear();
		CanAttackToPlayer = false;
		GetMesh()->GetAnimInstance()->Montage_Play(AttackAnim);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EAttackSound, GetMesh()->GetSocketLocation("Head"), GetMesh()->GetSocketRotation("Head"));
	}
	if (CharacterRef && !HitStun)
	{
		float CharacterHealth = CharacterRef->GetHealth();
		if (CharacterHealth <= 0)
		{
			//MoveToRandomLocation();
			GetWorldTimerManager().ClearTimer(TH_Tick);
		}
	}
}

//Donma
void ACPP_EnemyMonster::Freeze()
{ 
	IsFreeze = true;
	StaticMeshComponent->SetVisibility(true);
	GetMesh()->SetMaterial(0, FreezeMaterial);
	GetCharacterMovement()->DisableMovement();
}

//Donma durumundan cikma
void ACPP_EnemyMonster::UnFreeze()
{ 
	IsFreeze = false;
	StaticMeshComponent->SetVisibility(false);
	GetMesh()->SetMaterial(0, DefaultMaterial);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

//Hayatta olup olmadigini kontrol etme
void ACPP_EnemyMonster::Death()
{ 
	if (Health <= 0 && !IsDead)
	{
		IsDead = true;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		GetCharacterMovement()->DisableMovement();
		EnemySoundController->SetActive(false);
		UGameplayStatics::PlaySoundAtLocation(this, EDeathSound, GetMesh()->GetSocketLocation("Head"), GetMesh()->GetSocketRotation("Head"));
		FTimerHandle TH_ParticleDelay;
		GetWorldTimerManager().SetTimer(TH_ParticleDelay, this, &ACPP_EnemyMonster::ParticleDelay, 3.f, false);
	}
}

void ACPP_EnemyMonster::ParticleDelay()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetMesh()->GetBoneLocation("Hips"), FRotator(0.f, 0.f, 90.f));
	FTimerHandle TH_DestroyDelay;
	GetWorldTimerManager().SetTimer(TH_DestroyDelay, this, &ACPP_EnemyMonster::DestroyDelay, 0.7f, false);
}

void ACPP_EnemyMonster::DestroyDelay()
{
	HealthDropRandomSelector = UKismetMathLibrary::RandomInteger(2);
	if (HealthDropRandomSelector != 1)
	{
		FVector SpawnLocation = GetMesh()->GetSocketLocation("Hips");
		FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
		FActorSpawnParameters Parameters;
		Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(HealthDropClass, SpawnLocation, SpawnRotation, Parameters);
	}
	if (SpawnPointRef)
	{
		SpawnPointRef->ScoreCount++;
		SpawnPointRef->ActivePawns.Remove(this);
	}
	Destroy();
}

//Anim notify tarafindan tetiklenen fonksiyon
void ACPP_EnemyMonster::TriggerMonsterAttack()
{ 
	GetWorldTimerManager().SetTimer(TH_StartAttackTimer, this, &ACPP_EnemyMonster::StartAttackTrace, 0.01f, true);
}

//Zaman ayarli atak trace cizdirme
void ACPP_EnemyMonster::StartAttackTrace()
{ 
	FHitResult HitResult;
	FVector StartLoc = GetMesh()->GetSocketLocation("TraceStart");
	FVector EndLoc = GetMesh()->GetSocketLocation("TraceEnd");
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Camera, Params);
	if (HitResult.GetActor())
	{
		CharacterRef = Cast<ACPP_GodOfWarCharacter>(HitResult.GetActor());
		if (CharacterRef && CanDamage)
		{
			CanDamage = false;
			CharacterRef->GettingAnyDamageEvent(MonsterHandDamage);
			CharacterRef->GetDamageFromEnemy();
		}
	}
}

//Anim notify tarafindan tetiklenen atagi durdurma fonksiyonu
void ACPP_EnemyMonster::StopAttackTrace()
{ 
	CanDamage = true;
	CanAttackToPlayer = true;
	AIC_Ref->ReceiveMoveCompleted.Clear();
	GetWorldTimerManager().ClearTimer(TH_StartAttackTimer);
}

//Dusman bir saldiri aldiginda belli bir sure beklemesi
void ACPP_EnemyMonster::Stun()
{ 
	HitStun = true;
	CanAttackToPlayer = false;
	GetWorldTimerManager().SetTimer(TH_HitReactionDelay,this, &ACPP_EnemyMonster::WaitForNextAttack, 0.7f, false);
}

//Sure dolduktan sonra calisan ve yeniden saldirabilmesini saglayan fonksiyon
void ACPP_EnemyMonster::WaitForNextAttack()
{ 
	HitStun = false;
	CanAttackToPlayer = true;
	GetWorldTimerManager().ClearTimer(TH_HitReactionDelay);
}

void ACPP_EnemyMonster::RagdollFunction(float RagdollOverTime, bool IsJump)
{
	GetMesh()->SetSimulatePhysics(true);
	GetWorldTimerManager().SetTimer(TH_RagdollTrace, this, &ACPP_EnemyMonster::RagdollTrace, 0.01f, true);
	HitStun = true;
	CanAttackToPlayer = false;
	IsGetDamage = false;
	RagdollTime = RagdollOverTime;
	if (IsJump)
	{
		IsJump = false;
		GetMesh()->AddForce(FVector(0.f, 0.f, 12000000.f));
		GetCharacterMovement()->DisableMovement();
	}
	IsFall(true);
}

void ACPP_EnemyMonster::RagdollTrace()
{
	GetCapsuleComponent()->SetWorldLocation(GetMesh()->GetSocketLocation("Hips"));
	FHitResult HitResult;
	FVector Start = GetMesh()->GetSocketLocation("Hips");
	FVector End = (GetMesh()->GetSocketRotation("Hips").RotateVector(GetActorRightVector()) * 50.f) + Start;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, QueryParams);
	if (HitResult.bBlockingHit)
	{
		Front = true;
	}
	else
	{
		Front = false;
	}
}

void ACPP_EnemyMonster::IsFall(bool BeginFall)
{
	if (BeginFall)
	{
		BeginFall = false;
		if (KratosRotation.Yaw != NULL)
		{
			case_1 = KratosRotation.Yaw >= -180.f && KratosRotation.Yaw < -150.f;
			case_2 = KratosRotation.Yaw > -150.f && KratosRotation.Yaw < -120.f;
			case_3 = KratosRotation.Yaw > -120.f && KratosRotation.Yaw < -70.f;
			case_4 = KratosRotation.Yaw > -70.f && KratosRotation.Yaw < -30.f;
			case_5 = KratosRotation.Yaw > -30.f && KratosRotation.Yaw <= 0.f;
			case_6 = KratosRotation.Yaw < 30.f && KratosRotation.Yaw >= 0.f;
			case_7 = KratosRotation.Yaw < 70.f && KratosRotation.Yaw > 30.f;
			case_8 = KratosRotation.Yaw < 120.f && KratosRotation.Yaw >= 70.f;
			case_9 = KratosRotation.Yaw < 150.f && KratosRotation.Yaw >= 120.f;
			case_10 = KratosRotation.Yaw <= 180.f && KratosRotation.Yaw >= 150.f;

			if (case_1)
			{
				GetMesh()->AddForce(FVector((Force_x * -1.f), 0.f, 0.f));
			}
			else if (case_2)
			{
				GetMesh()->AddForce(FVector((Force_x * -1.f), (Force_y * -1.f), 0.f));
			}
			else if (case_3)
			{
				GetMesh()->AddForce(FVector(0.f, (Force_y * -1.f), 0.f));
			}
			else if (case_4)
			{
				GetMesh()->AddForce(FVector(Force_x, (Force_y * -1.f), 0.f));
			}
			else if (case_5)
			{
				GetMesh()->AddForce(FVector(Force_x, 0.f, 0.f));
			}
			else if (case_6)
			{
				GetMesh()->AddForce(FVector(Force_x, 0.f, 0.f));
			}
			else if (case_7)
			{
				GetMesh()->AddForce(FVector(Force_x, Force_y, 0.f));
			}
			else if (case_8)
			{
				GetMesh()->AddForce(FVector(0.f, Force_y, 0.f));
			}
			else if (case_9)
			{
				GetMesh()->AddForce(FVector((Force_x * -1.f), Force_y, 0.f));
			}
			else if (case_10)
			{
				GetMesh()->AddForce(FVector((Force_x * -1.f), 0.f, 0.f));
			}
			else
			{
				GetCapsuleComponent()->AddForce(FVector(0.f, 0.f, 0.f));
			}
			KratosRotation.Yaw = NULL;
		}
		//GetMesh()->GetAnimInstance()->Montage_Play(GetKickFall);
		GetWorldTimerManager().ClearTimer(TH_StartAttackTimer);
		GetWorldTimerManager().SetTimer(TH_GettingUp, this, &ACPP_EnemyMonster::GetUpAnims, RagdollTime, false);
	}
}

void ACPP_EnemyMonster::GetUpAnims()
{
	GetWorldTimerManager().ClearTimer(TH_RagdollTrace);
	GetMesh()->SetSimulatePhysics(false);
	if (Front)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(StandUpFrontAnim);
	}
	else
	{
		GetMesh()->GetAnimInstance()->Montage_Play(StandUpBackAnim);
	}
	float newRot_z = GetMesh()->GetComponentRotation().Yaw;
	GetCapsuleComponent()->SetWorldRotation(FRotator(0.f, newRot_z, 0.f));
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	FTimerHandle TH_Timer;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetWorldTimerManager().SetTimer(TH_Timer, this, &ACPP_EnemyMonster::DisableMove, 0.1f, false);
	GetWorldTimerManager().SetTimer(TH_CanMove, this, &ACPP_EnemyMonster::GetUpDefaultValue, 2.53f, false);
	GetWorldTimerManager().ClearTimer(TH_GettingUp);
}

void ACPP_EnemyMonster::GetUpDefaultValue()
{
	IsGetDamage = true;
	CanDamage = true;
	HitStun = false;
	CanAttackToPlayer = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetWorldTimerManager().ClearTimer(TH_CanMove);
}

void ACPP_EnemyMonster::DisableMove()
{
	GetCharacterMovement()->DisableMovement();
}


