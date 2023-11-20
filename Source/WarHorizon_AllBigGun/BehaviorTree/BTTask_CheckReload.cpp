// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_CheckReload.h"
#include "TurretAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckReload::UBTTask_CheckReload()
{
}

EBTNodeResult::Type UBTTask_CheckReload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	bool bIsReload = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISRELOAD);

	if (bIsReload)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
