// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimNotify.h"

#include "CPP_GodOfWarCharacter.h"

void UAttackAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ACPP_GodOfWarCharacter* Player = Cast<ACPP_GodOfWarCharacter>(MeshComp->GetOwner());
		if (Player != NULL)
		{
			Player->HandIsRight = IsRight;
			Player->TriggerAttackEvent();
		}
	}
}

void UAttackAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ACPP_GodOfWarCharacter* Player = Cast<ACPP_GodOfWarCharacter>(MeshComp->GetOwner());
		if (Player != NULL)
		{
			Player->StopAttackEvent();
		}
	}	
}
