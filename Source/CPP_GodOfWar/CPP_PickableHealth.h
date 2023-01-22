// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_PickableBase.h"
#include "CPP_PickableHealth.generated.h"

/**
 * 
 */
UCLASS()
class CPP_GODOFWAR_API ACPP_PickableHealth : public ACPP_PickableBase
{
	GENERATED_BODY()


private:

	float AddAmountHealth = 20.f;
	bool IsSettingHealth;

public:
	
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;

	virtual void Interact() override;
	
};
