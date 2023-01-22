// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "CPP_EnemyMonster.generated.h"

UCLASS()
class CPP_GODOFWAR_API ACPP_EnemyMonster : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* ArrowChr;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* ArrowDmg;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

public:
	// Sets default values for this character's properties
	ACPP_EnemyMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void InvestigateTrace();

	UFUNCTION()
	void MoveToRandomLocation();

	UFUNCTION()
	void StartChasePlayer();

	UFUNCTION()
	void ChasePlayer();

	UFUNCTION()
	void AttackPlayer(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UFUNCTION()
	void Freeze();

	UFUNCTION()
	void UnFreeze();

	UFUNCTION()
	void MoveComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UFUNCTION()
	void Death();

	void ParticleDelay();

	void DestroyDelay();

	void TriggerMonsterAttack();

	void StartAttackTrace();

	void StopAttackTrace();

	void Stun();

	void WaitForNextAttack();

	UFUNCTION()
	void IsFall(bool BeginFall);

	UFUNCTION()
	void GetUpAnims();

	UFUNCTION()
	void RagdollFunction(float RagdollOverTime, bool JumpRagdoll);

	UFUNCTION()
	void RagdollTrace();

	UFUNCTION()
	void GetUpDefaultValue();

	UFUNCTION()
	void DisableMove();

public:
	FTimerHandle TH_GettingUp;
	FTimerHandle TH_StartAttackTimer;
	FTimerHandle TH_Tick;
	FTimerHandle TH_HitReactionDelay;
	FTimerHandle TH_Lerp;
	FTimerHandle TH_CanMove;
	FTimerHandle TH_RagdollTrace;
	FTimerHandle TH_MoveLoop;

	bool HitStun = false;

	bool MoveRandomLocBool = true;
	bool Front;
	bool CanAttackToPlayer = true;

	UPROPERTY(EditDefaultsOnly)
	float Force_x;

	UPROPERTY(EditDefaultsOnly)
	float Force_y;

	float RagdollTime;

	int HealthDropRandomSelector;

	UPROPERTY(VisibleAnywhere)
	FRotator KratosRotation;

	UPROPERTY(VisibleAnywhere)
	FVector NewCapsuleLocation;

	bool case_1;
	bool case_2;
	bool case_3;
	bool case_4;
	bool case_5;
	bool case_6;
	bool case_7;
	bool case_8;
	bool case_9;
	bool case_10;

	float RotationAlpha;

	UPROPERTY(VisibleAnywhere)
	FRotator DefaultCapsuleRotation;

	UPROPERTY(EditDefaultsOnly)
	float Health = 100.f;

	UPROPERTY(EditDefaultsOnly)
	float MonsterHandDamage = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsFreeze;

	UPROPERTY(EditDefaultsOnly)
	bool CanDamage = true;

	UPROPERTY(EditDefaultsOnly)
	bool IsGetDamage = true;

	UPROPERTY(EditDefaultsOnly)
	bool SeeingPlayer = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Investigate = true;

	UPROPERTY(VisibleAnywhere)
	class AAIController* AIC_Ref;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* FreezeMaterial;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* DefaultMaterial;

	UPROPERTY(EditDefaultsOnly)
	class ACPP_GodOfWarCharacter* CharacterRef;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* HitReactionEnemy;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* GetKickFall;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* StandUpBackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* StandUpFrontAnim;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* DeathEffect;

	UPROPERTY(EditAnywhere)
	class ACPP_EnemySpawner* SpawnPointRef;

	UPROPERTY(EditDefaultsOnly, Category = Drops)
	UClass* HealthDropClass;

	UPROPERTY()
	UAudioComponent* EnemySoundController;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* EnemySound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* EDeathSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* EHitReactionSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* EAttackSound;

public:
	FORCEINLINE class UArrowComponent* GetCharacterArrow() { return ArrowChr; }
	FORCEINLINE class UArrowComponent* GetDamageArrow() { return ArrowDmg; }

	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE void SetHealth(float NewHealth) { Health = NewHealth; }

	FORCEINLINE void GetAnyDamage(float Damage) {
		if (IsGetDamage)
		{
			Health -= Damage;
		}
	}
};
