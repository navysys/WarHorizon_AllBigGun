// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTService_AircraftChangeHeight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/AircraftsInterface.h"
#include "BehaviorTree/AircraftsAI.h"

void UBTService_AircraftChangeHeight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}

	IAircraftsInterface* AircraftsPawn = Cast<IAircraftsInterface>(ControllingPawn);
	if (AircraftsPawn == nullptr)
	{
		return;
	}

	float TargetZ = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TARGETPOSITION).Z;
	float CurrentZ = ControllingPawn->GetActorLocation().Z;
	float Surplus = 100.0f;

	if (TargetZ + Surplus > CurrentZ)
	{
		AircraftsPawn->IncreaseHeight(TargetZ);
	}
	else if (TargetZ - Surplus < CurrentZ)
	{
		AircraftsPawn->DecreaseHeight(TargetZ);
	}
}
