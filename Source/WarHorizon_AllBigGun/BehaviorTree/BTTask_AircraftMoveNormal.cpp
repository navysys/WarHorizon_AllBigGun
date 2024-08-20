// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_AircraftMoveNormal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AircraftsAI.h"
#include "AIController.h"
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

	return EBTNodeResult::Failed;
}

void UBTTask_AircraftMoveNormal::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn)
	{
		FVector Target = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TARGETPOSITION);
		FVector Loc = ControllingPawn->GetActorLocation();
		Target.Z = 0;
		Loc.Z = 0;

		// 목표 위치에 도달하였는지 dist 로 판단
		float Dist = FVector::Dist(Target, Loc);
		if (Dist < 100.0f)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(BBKEY_AIRCRAFTSSTATE, static_cast<uint8>(EAircraftsState::Wait));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

	EAircraftsState AircraftsState = static_cast<EAircraftsState>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BBKEY_AIRCRAFTSSTATE));
	if (AircraftsState != EAircraftsState::MoveNormal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
