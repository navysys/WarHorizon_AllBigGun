// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_AircraftWaitState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AircraftsAI.h"
#include "AIController.h"
#include "Enum/EAircraftsState.h"

UBTTask_AircraftWaitState::UBTTask_AircraftWaitState()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_AircraftWaitState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FVector WaitingPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_WAITINGPOSITION);
	FVector TargetPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TARGETPOSITION);
	if (WaitingPos == FVector(0, 0, 0))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_WAITINGPOSITION, TargetPos);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_AircraftWaitState::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EAircraftsState AircraftsState = static_cast<EAircraftsState>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BBKEY_AIRCRAFTSSTATE));
	if (AircraftsState != EAircraftsState::Wait)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
