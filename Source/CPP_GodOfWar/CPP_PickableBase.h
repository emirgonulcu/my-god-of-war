// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "CPP_PickableBase.generated.h"

UCLASS()
class CPP_GODOFWAR_API ACPP_PickableBase : public AActor, public IInteractInterface 
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InteractCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* HealthMesh;
	
public:	
	// Sets default values for this actor's properties
	ACPP_PickableBase();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Interact() override;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* HPickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<class UUserWidget> wPickUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	UUserWidget* PickUpWidget;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* HealthParticleEffect;

	UPROPERTY(VisibleAnywhere)
	class ACPP_GodOfWarCharacter* CharacterRef;

};
