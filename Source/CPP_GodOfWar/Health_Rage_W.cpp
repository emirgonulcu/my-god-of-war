// Fill out your copyright notice in the Description page of Project Settings.


#include "Health_Rage_W.h"

#include "CPP_GodOfWarCharacter.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

void UHealth_Rage_W::NativeConstruct()
{
	Super::NativeConstruct();

	CharRef = Cast<ACPP_GodOfWarCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (CharRef)
	{
		CurrentHealth = CharRef->GetHealth() / 100.f;
		CurrentRage = CharRef->GetRage() / 100.f;
	}

	HealthBarPB->SetPercent(CurrentHealth);
	RageBarPB->SetPercent(CurrentRage);
}

void UHealth_Rage_W::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (UpdateHealth)
	{
		if (NewHealth != CurrentHealth)
		{
			CurrentHealth = FMath::Lerp(NewHealth, CurrentHealth, 0.6f);
			HealthBarPB->SetPercent(CurrentHealth);
		}
		else
		{
			UpdateHealth = false;
		}
	}
	if (UpdateRage)
	{
		if (NewRage != CurrentRage)
		{
			CurrentRage = FMath::Lerp(NewRage, CurrentRage, 0.6f);
			RageBarPB->SetPercent(CurrentRage);
		}
		else
		{
			UpdateRage = false;
		}
	}
}

void UHealth_Rage_W::UpdateHealthRage(float Health, float Rage)
{
	if (Health)
	{
		NewHealth = Health;
		UpdateHealth = true;
	}
	if (Rage)
	{
		NewRage = Rage;
		UpdateRage = true;
	}
}
