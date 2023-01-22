// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_MainHUD_W.generated.h"

/**
 * 
 */
UCLASS()
class CPP_GODOFWAR_API UCPP_MainHUD_W : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	class UHealth_Rage_W* HealthRageBarW;

	FORCEINLINE class UHealth_Rage_W* GetHealthRageRef() { return HealthRageBarW; }
	
};
