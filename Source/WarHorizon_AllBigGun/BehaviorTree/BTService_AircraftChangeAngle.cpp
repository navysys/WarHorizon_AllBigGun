// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTService_AircraftChangeAngle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/AircraftsInterface.h"
#include "BehaviorTree/AircraftsAI.h"

void UBTService_AircraftChangeAngle::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	FVector Target = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TARGETPOSITION);
	FVector Loc = ControllingPawn->GetActorLocation();
	FVector Dir = (Target - Loc).GetSafeNormal2D();
	
	float Yaw = Dir.Rotation().Yaw;
	//UE_LOG(LogTemp, Warning, TEXT("Rot : %f"), Yaw);
	AircraftsPawn->Turn(Yaw);
}
