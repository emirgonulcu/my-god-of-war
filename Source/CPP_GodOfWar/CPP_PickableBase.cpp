// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PickableBase.h"

#include "CPP_EnemyMonster.h"
#include "CPP_GodOfWarCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"

// Sets default values
ACPP_PickableBase::ACPP_PickableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(DefaultSceneRoot);

	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(GetRootComponent());
	
	HealthMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthMeshComponent"));
	HealthMesh->SetupAttachment(GetRootComponent());

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_PickableBase::OnComponentBeginOverlap);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &ACPP_PickableBase::OnComponentEndOverlap);
}

void ACPP_PickableBase::BeginPlay()
{
	Super::BeginPlay();
	if (wPickUp)
	{
		PickUpWidget = CreateWidget<UUserWidget>(this->GetGameInstance(), wPickUp);
	}
}

void ACPP_PickableBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		CharacterRef = Cast<ACPP_GodOfWarCharacter>(OtherActor);
		if (CharacterRef)
		{
			if (PickUpWidget)
			{
				if (!PickUpWidget->IsInViewport())
				{
					PickUpWidget->AddToViewport(9);
				}
			}
			CharacterRef->CanInteractObject = true;
			CharacterRef->PickableRef = this;
		}
	}
}

void ACPP_PickableBase::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		CharacterRef = Cast<ACPP_GodOfWarCharacter>(OtherActor);
		if (CharacterRef)
		{
			if (PickUpWidget)
			{
				if (PickUpWidget->IsInViewport())
				{
					PickUpWidget->RemoveFromParent();
				}
			}
			CharacterRef->CanInteractObject = false;
			CharacterRef->PickableRef = NULL;
		}
	}
}

void ACPP_PickableBase::Interact()
{
	
}

