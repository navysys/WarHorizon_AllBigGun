// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_GetAttackType.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/TurretInterface.h"
#include "AIController.h"
#include "TurretAI.h"

UBTTask_GetAttackType::UBTTask_GetAttackType()
{
}

EBTNodeResult::Type UBTTask_GetAttackType::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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

	uint8 AttackType = TurretPawn->GetAttackType();
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_ATTACKTYPE, AttackType);

	return EBTNodeResult::Succeeded;
}
