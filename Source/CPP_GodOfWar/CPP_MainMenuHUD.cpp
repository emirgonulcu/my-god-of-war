// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MainMenuHUD.h"

#include "CPP_GodOfWarCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ACPP_MainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (wMainMenuHUD)
	{
		MainMenuHUD = CreateWidget<UUserWidget>(this->GetGameInstance(), wMainMenuHUD);
		if (MainMenuHUD)
		{
			MainMenuHUD->AddToViewport(10);
		}
	}

	CharacterRef = Cast<ACPP_GodOfWarCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (CharacterRef)
	{
		CharacterRef->IsInMainMenu = true;
	}
	ControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	SetInputModeUIOnly();
}

void ACPP_MainMenuHUD::SetInputModeGameOnly()
{
	if (ControllerRef)
	{
		ControllerRef->SetInputMode(FInputModeGameOnly());
		ControllerRef->SetShowMouseCursor(false);
	}
}

void ACPP_MainMenuHUD::SetInputModeUIOnly()
{
	if (ControllerRef)
	{
		ControllerRef->SetInputMode(FInputModeUIOnly());
		ControllerRef->SetShowMouseCursor(true);
	}
}


