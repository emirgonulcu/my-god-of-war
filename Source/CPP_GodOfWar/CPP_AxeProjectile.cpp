// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AxeProjectile.h"
#include "CPP_EnemyMonster.h"
#include "CPP_GodOfWarCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

// Sets default values
ACPP_AxeProjectile::ACPP_AxeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    SetRootComponent(DefaultSceneRoot);

    AxeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AxeMesh"));
    AxeMeshComponent->SetupAttachment(GetRootComponent());
    SetRootComponent(AxeMeshComponent);
    
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
    SphereComponent->SetupAttachment(GetRootComponent());
    SphereComponent->InitSphereRadius(15.0f);

    SetRootComponent(DefaultSceneRoot);
    
    // Use this component to drive this projectile's movement.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    //ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 3000.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.3f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
    RotatingMovement->SetUpdatedComponent(AxeMeshComponent);
}

// Called when the game starts or when spawned
void ACPP_AxeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_AxeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //Balta firlatildiginda bir yere carpip carpmadigini kontrol etme
    if (!HitSomewhere)
    { 
	    FHitResult HitResult;
	    FVector Start = SphereComponent->GetComponentLocation();
	    FVector End = SphereComponent->GetComponentLocation() + FVector(10.f, 0.f, 0.f);
	    FCollisionQueryParams Params;
	    Params.AddIgnoredActor(this);
	    GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params);
		if (HitResult.bBlockingHit)
		{ //Bir yere carptiginda hareketini durdurma
            ProjectileMovementComponent->StopMovementImmediately();
            ProjectileMovementComponent->ProjectileGravityScale = 0.f;
            RotatingMovement->RotationRate = UE::Math::TRotator<double>(0.f,0.f,0.f);
			HitSomewhere = true;
			if (HitResult.GetActor())
            {
                MonsterRef = Cast<ACPP_EnemyMonster>(HitResult.GetActor());
                if (MonsterRef)
                { //Carptigi actor dusman ise dusmaný dondurma
                    MonsterRef->GetAnyDamage(ThrowDamage);
                    MonsterRef->Freeze();
                    AxeMeshComponent->SetVisibility(false);
                    UGameplayStatics::PlaySoundAtLocation(GetWorld(), AxeFreezeEnemySound, MonsterRef->GetActorLocation());
                }
            }
		}
    }
}

//Baltayi karaktere dondurmek
void ACPP_AxeProjectile::Return()
{ 
	if (!IsReturned)
	{
        CharacterRef = Cast<ACPP_GodOfWarCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (CharacterRef)
        {
	        if (MonsterRef)
	        {
                MonsterRef->UnFreeze();
                AxeMeshComponent->SetVisibility(true);
	        }
            CharacterRef->AxeCatchIdle = true;
            IsReturned = true;
            FVector Loc = CharacterRef->GetMesh()->GetComponentLocation();
            FRotator Rot = CharacterRef->GetMesh()->GetComponentRotation();
            CharacterRef->GetCharacterMovement()->DisableMovement();
            FLatentActionInfo LatentInfo;
            LatentInfo.CallbackTarget = this;
            TEnumAsByte<EMoveComponentAction::Type> ActionMode = EMoveComponentAction::Type::Move;
            UGameplayStatics::PlaySound2D(GetWorld(), AxeBackSound);
        	UKismetSystemLibrary::MoveComponentTo(DefaultSceneRoot, Loc, Rot * -1.f, false, true, OverTime, 
                false, ActionMode, LatentInfo);
        }
        GetWorldTimerManager().SetTimer(TH_DestroyDelay, this, &ACPP_AxeProjectile::WaitReturn, 0.45f, false);
	}
}

//Baltanin karaktere donmesini bekleme ve dondurme
void ACPP_AxeProjectile::WaitReturn()
{
    CharacterRef->AxeCatchIdle = false;
    CharacterRef->GetMesh()->GetAnimInstance()->Montage_Play(CharacterRef->AxeCatchAnim);
    CharacterRef->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    HitSomewhere = false;
    IsReturned = true;
    CharacterRef->ReappearingAxe();
    GetWorldTimerManager().ClearTimer(TH_DestroyDelay);
    Destroy();
}

