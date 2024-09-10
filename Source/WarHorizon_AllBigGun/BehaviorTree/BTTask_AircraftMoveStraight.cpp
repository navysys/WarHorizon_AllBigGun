// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_AircraftMoveStraight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AircraftsAI.h"
#include "Enum/EAircraftsState.h"

UBTTask_AircraftMoveStraight::UBTTask_AircraftMoveStraight()
{

}

EBTNodeResult::Type UBTTask_AircraftMoveStraight::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Succeeded;
}

void UBTTask_AircraftMoveStraight::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EAircraftsState AircraftsState = static_cast<EAircraftsState>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BBKEY_AIRCRAFTSSTATE));
	if (AircraftsState != EAircraftsState::MoveStraight)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISSTRAIGHTREADY, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
