// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_LevelScore_W.h"

#include <string>

#include "CPP_EnemySpawner.h"
#include "Components/TextBlock.h"

void UCPP_LevelScore_W::NativeConstruct()
{
	Super::NativeConstruct();

	LevelCountText->SetVisibility(ESlateVisibility::Hidden);
	if (EnemySpawnerRef)
	{
		WLevelTarget = EnemySpawnerRef->LevelTarget;
		FString LevelTargetString = FString::FromInt(WLevelTarget);
		LevelTargetText->SetText(FText::FromString(LevelTargetString));

		WLevelCount = EnemySpawnerRef->LevelCount;
		FString LevelCountString = FString::FromInt(WLevelCount);
		LevelCountString += ". ROUND COMPLETED";
		LevelCountText->SetText(FText::FromString(LevelCountString));

		WScoreCount = EnemySpawnerRef->ScoreCount;
		FString ScoreCountStr = FString::FromInt(WScoreCount);
		ScoreCountText->SetText(FText::FromString(ScoreCountStr));
	}
}

void UCPP_LevelScore_W::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (EnemySpawnerRef)
	{
		if (WScoreCount != EnemySpawnerRef->ScoreCount)
		{
			WScoreCount = EnemySpawnerRef->ScoreCount;
			FString ScoreCountStr = FString::FromInt(WScoreCount);
			ScoreCountText->SetText(FText::FromString(ScoreCountStr));
		}
		if (WLevelTarget != EnemySpawnerRef->LevelTarget)
		{
			WLevelTarget = EnemySpawnerRef->LevelTarget;
			FString LevelTargetString = FString::FromInt(WLevelTarget);
			LevelTargetText->SetText(FText::FromString(LevelTargetString));
		}
		if (WLevelCount != EnemySpawnerRef->LevelCount)
		{
			WLevelCount = EnemySpawnerRef->LevelCount;
			FString LevelCountString = FString::FromInt(WLevelCount);
			LevelCountString += ". LEVEL COMPLETED";
			LevelCountText->SetText(FText::FromString(LevelCountString));
		}
	}
}
