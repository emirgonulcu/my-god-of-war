// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MainMenu_W.h"

#include "CPP_GodOfWarCharacter.h"
#include "CPP_MainMenuHUD.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UCPP_MainMenu_W::NativeConstruct()
{
	Super::NativeConstruct();

	StartGame->OnClicked.AddDynamic(this, &UCPP_MainMenu_W::StartGameEvent);
	ShowControls->OnClicked.AddDynamic(this, &UCPP_MainMenu_W::ShowControlsEvent);
	ExitGame->OnClicked.AddDynamic(this, &UCPP_MainMenu_W::ExitGameEvent);

	ControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void UCPP_MainMenu_W::StartGameEvent()
{
	UGameplayStatics::OpenLevel(GetWorld(), "GothicTemple");
	ControllerRef->SetInputMode(FInputModeGameOnly());
}

void UCPP_MainMenu_W::ShowControlsEvent()
{
	if (ControlsWidgetClass)
	{
		Controls_W = CreateWidget<UUserWidget>(GetGameInstance(), ControlsWidgetClass); 
		if (!Controls_W->IsInViewport())
		{
			Controls_W->AddToViewport(9);
		}
	}
	RemoveFromParent();
}

void UCPP_MainMenu_W::ExitGameEvent()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
