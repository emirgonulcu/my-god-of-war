// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PickableHealth.h"

#include "CPP_GodOfWarCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void ACPP_PickableHealth::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_PickableHealth::Interact()
{
	float RandomExtraHealth = UKismetMathLibrary::RandomInteger(5);
	if (CharacterRef && !IsSettingHealth)
	{
		IsSettingHealth = true;
		AddAmountHealth += RandomExtraHealth;
		CharacterRef->SetNewHealth(AddAmountHealth);
		CharacterRef->CanInteractObject = false;
		CharacterRef->PickableRef = NULL;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HealthParticleEffect, GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlaySoundAtLocation(this, HPickSound, GetActorLocation());
		AddAmountHealth = 20.f;
		Destroy();
	}
}
