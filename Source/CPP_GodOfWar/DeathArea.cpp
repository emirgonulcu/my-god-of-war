// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathArea.h"

#include "CPP_EnemyMonster.h"
#include "CPP_GodOfWarCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ADeathArea::ADeathArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRootComponent"));
	SetRootComponent(DefaultSceneRoot);

	DeathCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathAreaComponent"));
	DeathCollision->SetupAttachment(GetRootComponent());

	DeathCollision->OnComponentBeginOverlap.AddDynamic(this, &ADeathArea::OnComponentBeginOverlap);
}

void ADeathArea::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		ACPP_GodOfWarCharacter* CharacterRef = Cast<ACPP_GodOfWarCharacter>(OtherActor);
		if (CharacterRef)
		{
			CharacterRef->GettingAnyDamageEvent(100.f);
		}
		ACPP_EnemyMonster* EnemyRef = Cast<ACPP_EnemyMonster>(OtherActor);
		if (EnemyRef)
		{
			EnemyRef->IsGetDamage = true;
			EnemyRef->GetAnyDamage(100.f);
		}
	}
}
