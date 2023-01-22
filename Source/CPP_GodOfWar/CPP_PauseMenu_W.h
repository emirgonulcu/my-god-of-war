// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_PauseMenu_W.generated.h"

/**
 * 
 */
UCLASS()
class CPP_GODOFWAR_API UCPP_PauseMenu_W : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeGame;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResumeText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MenuText;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void FResumeGame();
	
	UFUNCTION()
	void FMainMenu();

public:

	APlayerController* ControllerRef;

	UPROPERTY(VisibleAnywhere)
	class ACPP_GodOfWarCharacter* CharacterRef;

};
