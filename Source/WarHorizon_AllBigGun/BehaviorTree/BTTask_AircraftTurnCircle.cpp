// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_AircraftTurnCircle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AircraftsAI.h"
#include "AIController.h"

UBTTask_AircraftTurnCircle::UBTTask_AircraftTurnCircle()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_AircraftTurnCircle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector WaitingPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_WAITINGPOSITION);
	FVector TargetPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TARGETPOSITION);
	if (WaitingPos == FVector(0, 0, 0))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_WAITINGPOSITION, TargetPos);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_AircraftTurnCircle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	float WaitAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_WAITANGLE);
	FVector Center = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_WAITINGPOSITION); // ���� �߽�
	float RotationSpeed = 30.0f; // �ʱ� ȸ�� �ӵ� (�� ����)


	WaitAngle += RotationSpeed * DeltaSeconds;
	if (WaitAngle >= 180.0f)
	{
		RotationSpeed = 15.0f;	// ���� �ӵ� 1500 ���� 15���� ȸ���ϸ� �������� ���ڸ� ȸ��
	}

	FRotator Rot = ControllingPawn->GetActorRotation();
	Rot.Yaw += RotationSpeed * DeltaSeconds;

	ControllingPawn->SetActorRotation(Rot);
}
