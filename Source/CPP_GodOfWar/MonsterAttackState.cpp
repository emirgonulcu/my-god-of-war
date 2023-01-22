// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAttackState.h"

#include "CPP_EnemyMonster.h"

void UMonsterAttackState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ACPP_EnemyMonster* Enemy = Cast<ACPP_EnemyMonster>(MeshComp->GetOwner());
		if (Enemy != NULL)
		{
			Enemy->TriggerMonsterAttack();
		}
	}
}

void UMonsterAttackState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ACPP_EnemyMonster* Enemy = Cast<ACPP_EnemyMonster>(MeshComp->GetOwner());
		if (Enemy != NULL)
		{
			Enemy->StopAttackTrace();
		}
	}
}
