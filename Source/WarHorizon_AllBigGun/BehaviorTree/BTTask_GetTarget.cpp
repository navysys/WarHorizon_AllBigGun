// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_GetTarget.h"
#include "TurretAI.h"
#include "Interface/TurretInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetTarget::UBTTask_GetTarget()
{
}

EBTNodeResult::Type UBTTask_GetTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	APawn* TargetPawn = TurretPawn->GetTurretTarget();
	if (nullptr == TargetPawn)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, TargetPawn);

	return EBTNodeResult::Succeeded;
}
