// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_RestartMenu_W.h"

#include "CPP_GodOfWarCharacter.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UCPP_RestartMenu_W::NativeConstruct()
{
	Super::NativeConstruct();

	RestartGame->OnPressed.AddDynamic(this, &UCPP_RestartMenu_W::RestartingGame);
	BackToMainMenu->OnPressed.AddDynamic(this, &UCPP_RestartMenu_W::BackMainMenu);
	
	ControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (ControllerRef)
	{
		ControllerRef->SetInputMode(FInputModeUIOnly());
		ControllerRef->SetShowMouseCursor(true);
	}
	
	CharRef = Cast<ACPP_GodOfWarCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void UCPP_RestartMenu_W::RestartingGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), "GothicTemple");
	if (ControllerRef)
	{
		ControllerRef->SetInputMode(FInputModeGameOnly());
		ControllerRef->SetShowMouseCursor(false);
	}
}

void UCPP_RestartMenu_W::BackMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), "LevelWinterForest");
}
