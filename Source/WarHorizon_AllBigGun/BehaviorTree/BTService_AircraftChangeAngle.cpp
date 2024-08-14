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
	Target.Z = 0;
	Loc.Z = 0;
	FVector Dir = (Target - Loc).GetSafeNormal2D();

	// ��ǥ ��ġ�� �����Ͽ����� dist �� �Ǵ�

	bool IsMoveStraight = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISMOVESTRAIGHT);
	if (IsMoveStraight == false)
	{

	}

	float Dist = FVector::Dist(Target, Loc);
	if (Dist < 100.0f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISREACHTAGETPOSITION, true);
	}

	bool IsReachTargetPosition = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISREACHTAGETPOSITION);
	if (IsReachTargetPosition == false)
	{
		// ��ǥ �������� ȸ���ϵ��� Dir �� rotation ���� �Ǵ�
		int Angle = ControllingPawn->GetActorRotation().Yaw - Dir.Rotation().Yaw;
		if (Angle > 180)
		{
			Angle -= 360;
		}
		else if (Angle < -180)
		{
			Angle += 360;
		}

		AircraftsPawn->Turn(Angle);
	}
}
