// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_AircraftWaitState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AircraftsAI.h"
#include "AIController.h"

UBTTask_AircraftWaitState::UBTTask_AircraftWaitState()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_AircraftWaitState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	bool bIsReachTargetPosition = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISREACHTAGETPOSITION);
	if (bIsReachTargetPosition)
	{
		FVector WaitingPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_WAITINGPOSITION);
		FVector TargetPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TARGETPOSITION);
		if (WaitingPos == FVector(0, 0, 0))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_WAITINGPOSITION, TargetPos);
		}

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_AircraftWaitState::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool bIsReachTargetPosition = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISREACHTAGETPOSITION);
	if (bIsReachTargetPosition == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	static float Angle = 0.0f;
	float RotationSpeed = 30.0f; // 초기 회전 속도 (도 단위)
	FVector Center = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_WAITINGPOSITION); // 원의 중심
	FVector CurrentLocation = OwnerPawn->GetActorLocation();

	Angle += RotationSpeed * DeltaSeconds;
	if (Angle >= 180.0f)
	{
		RotationSpeed = 15.0f;	// 현재 속도 1500 기준 15도로 회전하면 원형으로 제자리 회전
	}

	FRotator Rot = OwnerPawn->GetActorRotation();
	Rot.Yaw += RotationSpeed * DeltaSeconds;

	OwnerPawn->SetActorRotation(Rot);
}
