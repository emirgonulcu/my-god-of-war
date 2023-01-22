// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "CPP_GodOfWarCharacter.generated.h"


UCLASS(config=Game)
class ACPP_GodOfWarCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* WeaponSocketSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rage, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* RageHandR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rage, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* RageHandL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Aim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReturnInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EquipInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* KickInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RageActiveInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RollInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PauseMenuInputAction;

public:
	ACPP_GodOfWarCharacter();
	

protected:
	virtual void Tick(float DeltaSeconds) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void AttackInputEvent();

	UFUNCTION()
	void StartSprint();

	UFUNCTION()
	void Walk();

	UFUNCTION()
	void SetDefaultMovement();
public:

	UFUNCTION()
	void FPauseGame();

	UFUNCTION()
	void FUnPauseGame();

	UFUNCTION()
	void WallTrace();

	UFUNCTION()
	void InteractEvent();

	UFUNCTION()
	void RollAnimationStart();

	UFUNCTION()
	void RollDelay();

	UFUNCTION()
	void RollActive();

	UFUNCTION()
	void TriggerAttackEvent();

	UFUNCTION()
	void GetDamageFromEnemy();

	UFUNCTION()
	void TriggerAttack();

	UFUNCTION()
	void StopAttackEvent();

	UFUNCTION()
	void StoppingAttack();

	UFUNCTION()
	void KickEvent();

	UFUNCTION()
	void KickTriggerEvent();

	UFUNCTION()
	void StartKickAttack();

	UFUNCTION()
	void StopKickEvent();

	UFUNCTION()
	void StartRage();

	UFUNCTION()
	void RageControl();

	UFUNCTION()
	void StopRage();

	UFUNCTION()
	void RegenerateHealth();

	UFUNCTION()
	void RegenerateRage();

	UFUNCTION()
	void AimAction();

	UFUNCTION()
	void StopAim();

	UFUNCTION()
	void DissappearingAxe();

	UFUNCTION()
	void ReturnAxe();

	UFUNCTION()
	void ReappearingAxe();

	UFUNCTION()
	void EquipUnequip();

	UFUNCTION()
	void EquipFunction();

	UFUNCTION()
	void UnequipFunction();

	UFUNCTION()
	void DeathControl();

	UFUNCTION()
	void JumpEnemy();

	UFUNCTION()
	void HealthRageUpdate();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

public:
	/*------------Public-Variables--------------*/
	UPROPERTY()
	bool IsBlockingWall;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ScoreCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool CanInteractObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Rage = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool CanAttack = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsAiming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsClose;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool AxeCatchIdle = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool CanKick = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool CanDamage = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool InHand = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsEquipped = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsPressed = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsDead = false;

	UPROPERTY(VisibleAnywhere)
	bool CanStartRage = true;

	UPROPERTY(VisibleAnywhere)
	bool IsInRage = false;

	UPROPERTY(VisibleAnywhere)
	bool IsRegenerateHealth = false;

	UPROPERTY(VisibleAnywhere)
	bool IsInMainMenu;

	UPROPERTY(VisibleAnywhere)
	bool IsPausedGame = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Footstep, meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* FootstepVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UNiagaraSystem* FireHand;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle TH_Attack;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle TH_Kick;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle TH_JumpEnemy;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle TH_RageCalculate;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle TH_RegenerateRage;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle TH_RegenerateHealth;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle TH_WaitNextAttack;
	/*------------------------------------------*/

private:
	/*------Variables-------------------*/
	UPROPERTY(VisibleAnywhere)
	FTimerHandle TH_Dissapear;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle TH_AttachTimer;
	
	UPROPERTY(VisibleAnywhere)
	float TraceTime = 0.01f;

	UPROPERTY(VisibleAnywhere)
	float ForwardDodge;

	UPROPERTY(VisibleAnywhere)
	float RightDodge;

	UPROPERTY(VisibleAnywhere)
	int HandAttackCount = 0;

	UPROPERTY(VisibleAnywhere)
	int AttackCount = 0;

	UPROPERTY(EditDefaultsOnly)
	float SprintingSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly)
	float WalkingSpeed = 460.f;

public:

	UPROPERTY(EditDefaultsOnly)
	float AxeDamage = 20.f;

	UPROPERTY(EditDefaultsOnly)
	float HandDamage = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float ActiveRageDamage = 5.f;

	UPROPERTY(EditDefaultsOnly)
	float AxeThrowDamage = 10.f;

private:

	UPROPERTY(EditDefaultsOnly)
	class ACPP_AxeProjectile* AxeProjectileRef;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* AxeThrowSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* PunchHitSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* PunchMissSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* FootstepSound;

	UPROPERTY(EditDefaultsOnly, Category=Sounds)
	USoundBase* HitEnemySound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* StartRageSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* StartRageKratosSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* RagePunchSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* StopRageSound;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* HitReactionSound;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* BloodParticle;
	/*------------------------------------------*/

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE float GetHealth() { return Health; }

	FORCEINLINE void SetNewHealth(float AddingHealth)
	{
		Health = FMath::Clamp(Health + AddingHealth, 0.f, 100.f);
	}

	FORCEINLINE float GetRage() { return Rage; }

	FORCEINLINE void GettingAnyDamageEvent(float Damage) { Health -= Damage; }

	UPROPERTY(EditDefaultsOnly, Category = Class)
	UClass* AxeProjectileClass;

	/*----------------Animasyonlar------------------*/

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	TArray<UAnimMontage*> AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AxeThrowAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AxeCatchAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* EquipAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* UnequipAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	TArray<UAnimMontage*> HandAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* HandMoveIdle;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* HitReaction;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* KickAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* RollForwardAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* RollBackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* RollRightAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* RollLeftAnim;

	/*---------------------------------------------*/
	UPROPERTY(EditAnywhere)
	bool HandIsRight;

	UPROPERTY(EditDefaultsOnly)
	bool IsDodging = false;

	/*------------------Referanslar-----------------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<class UUserWidget> wCrossHair;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	UUserWidget* CrossHair_W;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<class UUserWidget> wMainHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	UUserWidget* MainHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<class UUserWidget> wRestartMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	UUserWidget* Restart_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<class UUserWidget> wPauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	UUserWidget* Pause_Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	class UCPP_MainHUD_W* MainHUD_Ref;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= Widget)
	class UHealth_Rage_W* HealthRageBar_W;

	UPROPERTY(VisibleAnywhere)
	class ACPP_EnemyMonster* EnemyRef;

	UPROPERTY(VisibleAnywhere)
	FVector2D MovementVector;

	UPROPERTY(VisibleAnywhere)
	class ACPP_PickableBase* PickableRef;
};

