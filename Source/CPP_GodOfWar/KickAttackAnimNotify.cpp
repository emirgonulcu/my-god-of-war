// Fill out your copyright notice in the Description page of Project Settings.


#include "KickAttackAnimNotify.h"

#include "CPP_GodOfWarCharacter.h"

void UKickAttackAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ACPP_GodOfWarCharacter* Player = Cast<ACPP_GodOfWarCharacter>(MeshComp->GetOwner());
		if (Player != NULL)
		{
			Player->KickTriggerEvent();
		}
	}
}

void UKickAttackAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ACPP_GodOfWarCharacter* Player = Cast<ACPP_GodOfWarCharacter>(MeshComp->GetOwner());
		if (Player != NULL)
		{
			Player->StopKickEvent();
		}
	}
}
