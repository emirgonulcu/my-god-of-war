// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_EnemySpawner.generated.h"

UCLASS()
class CPP_GODOFWAR_API ACPP_EnemySpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Root, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultRootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SpawnBound;
	
public:	
	// Sets default values for this actor's properties
	ACPP_EnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION()
	void ReSpawnEnemy();

	UFUNCTION()
	void NewLevel();

	UPROPERTY()
	FTimerHandle TH_ReSpawnEnemy;

	UPROPERTY()
	FTimerHandle TH_SpawnLoop;

	UPROPERTY()
	FTimerHandle TH_StartNewLevel;

	UPROPERTY()
	bool IsStartNewLevelTimer = false;

	UPROPERTY(EditDefaultsOnly)
	int SpawnAmount;

	UPROPERTY()
	int EnemySelection = 0;

public:
	UPROPERTY()
	int LevelFinalSpawn;

	UPROPERTY()
	int LevelSpawnCount;

	UPROPERTY()
	int LevelCount = 1;

	UPROPERTY()
	int LevelTarget = 27;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RandSpawnMin = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RandSpawnMax = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeTillRespawn = 3.f;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ScoreCount = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACharacter> EnemyClass;

	UPROPERTY(VisibleAnywhere)
	ACharacter* Enemy;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AController> EnemyAIControllerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<APawn*> ActivePawns;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ACPP_EnemyMonster* EnemyRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ACPP_GodOfWarCharacter* CharacterRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<UUserWidget> WLevelScoreClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	UUserWidget* LevelScoreWidget;

	UPROPERTY()
	class UCPP_LevelScore_W* LevelScoreWRef;

};
