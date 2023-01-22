// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PauseMenu_W.h"

#include "CPP_GodOfWarCharacter.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UCPP_PauseMenu_W::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeGame->OnPressed.AddDynamic(this, &UCPP_PauseMenu_W::FResumeGame);
	MainMenuButton->OnPressed.AddDynamic(this, &UCPP_PauseMenu_W::FMainMenu);

	ControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (ControllerRef)
	{
		ControllerRef->SetInputMode(FInputModeGameAndUI());
		ControllerRef->SetShowMouseCursor(true);
		ControllerRef->SetPause(true);
	}

	CharacterRef = Cast<ACPP_GodOfWarCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void UCPP_PauseMenu_W::FResumeGame() 
{
	if (ControllerRef)
	{
		ControllerRef->SetPause(false);
		ControllerRef->SetInputMode(FInputModeGameOnly());
		ControllerRef->SetShowMouseCursor(false);
	}
	if (CharacterRef)
	{
		CharacterRef->IsPausedGame = false;
	}
	if (this->IsInViewport())
	{
		RemoveFromParent();
	}
}

void UCPP_PauseMenu_W::FMainMenu()
{
	if (ControllerRef)
	{
		ControllerRef->SetPause(false);
	}
	if (CharacterRef)
	{
		CharacterRef->IsPausedGame = false;
	}
	if (this->IsInViewport())
	{
		RemoveFromParent();
	}
	UGameplayStatics::OpenLevel(GetWorld(), "LevelWinterForest");
}