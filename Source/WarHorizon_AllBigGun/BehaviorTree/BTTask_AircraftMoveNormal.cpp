// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_AircraftMoveNormal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AircraftsAI.h"
#include "Enum/EAircraftsState.h"

UBTTask_AircraftMoveNormal::UBTTask_AircraftMoveNormal()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_AircraftMoveNormal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EAircraftsState AircraftsState = static_cast<EAircraftsState>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BBKEY_AIRCRAFTSSTATE));
	if (AircraftsState == EAircraftsState::MoveNormal)
	{
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Succeeded;
}

void UBTTask_AircraftMoveNormal::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EAircraftsState AircraftsState = static_cast<EAircraftsState>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BBKEY_AIRCRAFTSSTATE));
	if (AircraftsState != EAircraftsState::MoveNormal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
