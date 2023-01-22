// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Health_Rage_W.generated.h"

/**
 * 
 */
UCLASS()
class CPP_GODOFWAR_API UHealth_Rage_W : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBarPB;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* RageBarPB;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	void UpdateHealthRage(float Health, float Rage);

	FTimerHandle TH_HealthTimer;

	bool UpdateHealth = true;
	bool UpdateRage = true;

	float CurrentHealth;
	float CurrentRage;

	float NewHealth;
	float NewRage;

	UPROPERTY(VisibleAnywhere)
	class ACPP_GodOfWarCharacter* CharRef;
};
