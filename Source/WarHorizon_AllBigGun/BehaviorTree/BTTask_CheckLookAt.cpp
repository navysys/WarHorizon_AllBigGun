// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_CheckLookAt.h"
#include "TurretAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckLookAt::UBTTask_CheckLookAt()
{
}

EBTNodeResult::Type UBTTask_CheckLookAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	bool bIsLookAt = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISLOOKAT);

	if (bIsLookAt)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
