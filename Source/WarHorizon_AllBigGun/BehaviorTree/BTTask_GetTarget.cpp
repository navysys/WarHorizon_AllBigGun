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

	uint8 AttackType = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_ATTACKTYPE);
	if (AttackType == 0)
	{

	}
	else if(AttackType == 1)
	{
		float TargetAngle = TurretPawn->GetTurretTargetAngle();
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_TARGETANGLE, TargetAngle);
	}
	else if(AttackType == 2)
	{
		FVector TargetPoint = TurretPawn->GetTurretTargetPoint();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_POINTANGLE, TargetPoint);
	}
	else if (AttackType == 3)
	{
		APawn* TargetPawn = TurretPawn->GetTurretTargetPawn();
		if (nullptr == TargetPawn)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
			return EBTNodeResult::Failed;
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, TargetPawn);
	}

	

	return EBTNodeResult::Succeeded;
}
