// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTService_AircraftChangeHeight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UBTService_AircraftChangeHeight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	/*ITurretInterface* TurretPawn = Cast<ITurretInterface>(ControllingPawn);
	if (nullptr == TurretPawn)
	{
		return;
	}*/
}
