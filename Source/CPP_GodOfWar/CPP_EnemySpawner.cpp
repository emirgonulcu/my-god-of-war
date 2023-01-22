// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_EnemySpawner.h"

#include "CPP_EnemyMonster.h"
#include "CPP_GodOfWarCharacter.h"
#include "CPP_LevelScore_W.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPP_EnemySpawner::ACPP_EnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(DefaultRootScene);

	SpawnBound = CreateDefaultSubobject<UBoxComponent>(TEXT("Bound"));
	SpawnBound->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACPP_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (WLevelScoreClass)
	{
		LevelScoreWidget = CreateWidget<UUserWidget>(this->GetGameInstance(), WLevelScoreClass);
		LevelScoreWRef = Cast<UCPP_LevelScore_W>(LevelScoreWidget);
		if (LevelScoreWRef)
		{
			LevelScoreWRef->EnemySpawnerRef = this;
			LevelScoreWidget->AddToViewport(9);
		}
	}
	CharacterRef = Cast<ACPP_GodOfWarCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	SpawnEnemy();
	GetWorldTimerManager().SetTimer(TH_ReSpawnEnemy, this, &ACPP_EnemySpawner::ReSpawnEnemy, TimeTillRespawn, true);
}

void ACPP_EnemySpawner::SpawnEnemy()
{
	SpawnAmount = UKismetMathLibrary::RandomIntegerInRange(RandSpawnMin, RandSpawnMax);
	LevelSpawnCount += SpawnAmount;
	if (LevelSpawnCount > LevelTarget)
	{
		LevelFinalSpawn = LevelSpawnCount - LevelTarget;
		LevelSpawnCount = LevelSpawnCount - LevelFinalSpawn;
		SpawnAmount = LevelFinalSpawn;
	}
	for (size_t i = 1; i <= SpawnAmount; i++)
	{
		FHitResult HitResult;
		FBoxSphereBounds BoxInfo = SpawnBound->GetNavigationBounds();
		FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(BoxInfo.Origin, BoxInfo.BoxExtent);
		FVector StartPoint = FVector(RandomPoint.X, RandomPoint.Y, RandomPoint.Z + 10000.f);
		FVector EndPoint = StartPoint + (GetActorUpVector() * -1) * 15000.f;
		FCollisionObjectQueryParams Parameters;
		Parameters.AddObjectTypesToQuery(ECC_WorldStatic);
		GetWorld()->LineTraceSingleByObjectType(HitResult, RandomPoint, EndPoint, Parameters);
		if (HitResult.bBlockingHit)
		{
			RandomPoint = HitResult.Location;
		}
		RandomPoint.Z += 100.f;
		FRotator RandomRotation = FRotator(0.f, 0.f, UKismetMathLibrary::RandomFloat());
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		Enemy = GetWorld()->SpawnActor<ACharacter>(EnemyClass, RandomPoint, RandomRotation, Params);
		EnemyRef = Cast<ACPP_EnemyMonster>(Enemy);
		if (EnemyRef)
		{
			EnemyRef->AIControllerClass = EnemyAIControllerClass;
			EnemyRef->SpawnDefaultController();
			EnemyRef->SpawnPointRef = this;
			ActivePawns.Add(EnemyRef);
		}
	}
}

void ACPP_EnemySpawner::ReSpawnEnemy()
{
	if (ActivePawns.Num() == 0)
	{
		if (LevelSpawnCount == LevelTarget && !IsStartNewLevelTimer)
		{
			LevelScoreWRef->LevelCountText->SetVisibility(ESlateVisibility::Visible);
			IsStartNewLevelTimer = true;
			GetWorldTimerManager().SetTimer(TH_StartNewLevel, this, &ACPP_EnemySpawner::NewLevel, 15.f, false);
		}
		else if (LevelSpawnCount != LevelTarget)
		{
			SpawnEnemy();
		}
	}
}

void ACPP_EnemySpawner::NewLevel()
{
	LevelScoreWRef->LevelCountText->SetVisibility(ESlateVisibility::Hidden);
	RandSpawnMin += 1;
	RandSpawnMax += 1;
	LevelTarget += 24;
	LevelCount++;
	if (CharacterRef)
	{
		CharacterRef->AxeDamage += 5.f;
		CharacterRef->AxeThrowDamage += 2.f;
		CharacterRef->HandDamage += 4.f;
		CharacterRef->ActiveRageDamage += 5.f;
	}
	IsStartNewLevelTimer = false;
	GetWorldTimerManager().ClearTimer(TH_StartNewLevel);
}

