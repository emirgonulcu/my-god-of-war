// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CPP_AxeProjectile.generated.h"


UCLASS()
class CPP_GODOFWAR_API ACPP_AxeProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category= SceneComponent)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = StaticMeshComponent, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* AxeMeshComponent;

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere, Category = MovementComponent)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = MovementComponent)
	class URotatingMovementComponent* RotatingMovement;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComponent;
	
public:	
	// Sets default values for this actor's properties
	ACPP_AxeProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Return();

	void WaitReturn();

	void AxeRotateSoundLoop();
	
	bool HitSomewhere = false;

	bool IsReturned = true;

	FTimerHandle TH_DestroyDelay;
	FTimerHandle TH_AxeRotateLoop;

	UPROPERTY(EditDefaultsOnly)
	float ThrowDamage = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float OverTime = 0.45f;

	UPROPERTY(EditDefaultsOnly)
	class ACPP_GodOfWarCharacter* CharacterRef;

	UPROPERTY(EditDefaultsOnly)
	class ACPP_EnemyMonster* MonsterRef;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* AxeBackSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* AxeStartSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* AxeFreezeEnemySound;
};
