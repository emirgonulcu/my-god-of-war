// Fill out your copyright notice in the Description page of Project Settings.


#include "RollAnimNotifyState.h"

#include "CPP_GodOfWarCharacter.h"

void URollAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ACPP_GodOfWarCharacter* Player = Cast<ACPP_GodOfWarCharacter>(MeshComp->GetOwner());
		if (Player != NULL)
		{
			Player->RollDelay();
		}
	}
}
