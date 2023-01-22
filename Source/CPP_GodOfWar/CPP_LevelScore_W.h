// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_LevelScore_W.generated.h"

/**
 * 
 */
UCLASS()
class CPP_GODOFWAR_API UCPP_LevelScore_W : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ScoreCountText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* LevelTargetText;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelCountText;

	UPROPERTY()
	int WLevelTarget;

	UPROPERTY()
	int WLevelCount;

	UPROPERTY()
	int WScoreCount;

	UPROPERTY()
	class ACPP_EnemySpawner* EnemySpawnerRef;

};
