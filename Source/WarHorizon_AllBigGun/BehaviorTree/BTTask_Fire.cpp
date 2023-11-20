// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_Fire.h"
#include "TurretAI.h"
#include "AIController.h"
#include "Interface/TurretInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Fire::UBTTask_Fire()
{
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	ITurretInterface* TurretPawn = Cast<ITurretInterface>(ControllingPawn);
	if (nullptr == TurretPawn)
	{
		return EBTNodeResult::Failed;
	}

	TurretPawn->Fire();
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_RELOADTIME, TurretPawn->GetReloadTime());
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISLOOKAT, false);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISRELOAD, false);

	return EBTNodeResult::Succeeded;
}
