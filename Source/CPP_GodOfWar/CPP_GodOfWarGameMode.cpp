// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPP_GodOfWarGameMode.h"
#include "CPP_GodOfWarCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACPP_GodOfWarGameMode::ACPP_GodOfWarGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/CharacterBP/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
