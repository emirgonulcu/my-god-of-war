// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CPP_MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class CPP_GODOFWAR_API ACPP_MainMenuHUD : public AHUD
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	void SetInputModeGameOnly();

	void SetInputModeUIOnly();

public:
	APlayerController* ControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<class UUserWidget> wMainMenuHUD;
	
	UUserWidget* MainMenuHUD;

	UPROPERTY(VisibleAnywhere)
	class ACPP_GodOfWarCharacter* CharacterRef;
};
