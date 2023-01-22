// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_RestartMenu_W.generated.h"

/**
 * 
 */
UCLASS()
class CPP_GODOFWAR_API UCPP_RestartMenu_W : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UButton* RestartGame;

	UPROPERTY(meta=(BindWidget))
	class UButton* BackToMainMenu;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RestartText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MainMenuText;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void RestartingGame();

	UFUNCTION()
	void BackMainMenu();

public:
	APlayerController* ControllerRef;

	UPROPERTY(VisibleAnywhere)
	class ACPP_GodOfWarCharacter* CharRef;
};
