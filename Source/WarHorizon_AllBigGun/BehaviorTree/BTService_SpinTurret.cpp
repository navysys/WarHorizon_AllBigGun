// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTService_SpinTurret.h"
#include "TurretAI.h"
#include "Interface/TurretInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"

UBTService_SpinTurret::UBTService_SpinTurret()
{
}

void UBTService_SpinTurret::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	ITurretInterface* TurretPawn = Cast<ITurretInterface>(ControllingPawn);
	if (nullptr == TurretPawn)
	{
		return;
	}

	float RotationSpeed = TurretPawn->GetRotationSpeed();
	float CurrentYaw = ControllingPawn->GetActorRotation().Yaw;
	float MaxAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_MAXANGLE);


	uint8 AttackType = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ATTACKTYPE);
	if (AttackType == 0)
	{

	}
	else if (AttackType == 1)
	{

	}
	else if (AttackType == 2)
	{
		// 포인트 공격 모드
		float PointAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_POINTANGLE);

		//if(MaxAngle)

		float DeltaAngle = FMath::FindDeltaAngleDegrees<float>(CurrentYaw, PointAngle);
		float ChangeInRotation = FMath::Clamp(DeltaAngle, -RotationSpeed * DeltaSeconds, RotationSpeed * DeltaSeconds);

		ControllingPawn->AddActorLocalRotation(FRotator(0.0f, ChangeInRotation, 0.0f));

		if (static_cast<int>(PointAngle * 100) == static_cast<int>(CurrentYaw * 100))
		{
			ControllingPawn->SetActorRotation(FRotator(ControllingPawn->GetActorRotation().Pitch, PointAngle, ControllingPawn->GetActorRotation().Roll));
		}
	}
	else if(AttackType == 3)
	{
		// 타겟 공격 모드
		float TargetAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_TARGETANGLE);

		float DeltaAngle = FMath::FindDeltaAngleDegrees<float>(CurrentYaw, TargetAngle);
		float ChangeInRotation = FMath::Clamp(DeltaAngle, -RotationSpeed * DeltaSeconds, RotationSpeed * DeltaSeconds);

		ControllingPawn->AddActorLocalRotation(FRotator(0.0f, ChangeInRotation, 0.0f));

		if (static_cast<int>(TargetAngle * 100) == static_cast<int>(CurrentYaw * 100))
		{
			ControllingPawn->SetActorRotation(FRotator(ControllingPawn->GetActorRotation().Pitch, TargetAngle, ControllingPawn->GetActorRotation().Roll));
		}
	}
}
