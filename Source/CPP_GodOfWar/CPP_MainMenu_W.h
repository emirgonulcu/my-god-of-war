// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_MainMenu_W.generated.h"

/**
 * 
 */
UCLASS()
class CPP_GODOFWAR_API UCPP_MainMenu_W : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UButton* StartGame;

	UPROPERTY(meta = (BindWidget))
	class UButton* ShowControls;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitGame;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StartGameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ShowControlsText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ExitGameText;

	UPROPERTY(meta = (BindWidget))
	class UImage* Logo;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartGameEvent();

	UFUNCTION()
	void ShowControlsEvent();

	UFUNCTION()
	void ExitGameEvent();

public:
	APlayerController* ControllerRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> ControlsWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	UUserWidget* Controls_W;

};
