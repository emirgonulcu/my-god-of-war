// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_PickUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPP_GODOFWAR_API UCPP_PickUpWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PickText;
	
};
