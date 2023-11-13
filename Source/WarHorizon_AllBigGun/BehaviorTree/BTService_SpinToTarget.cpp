// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTService_SpinToTarget.h"
#include "TurretAI.h"
#include "Interface/TurretInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"

UBTService_SpinToTarget::UBTService_SpinToTarget()
{
}

void UBTService_SpinToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	float TargetAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_TARGETANGLE);
	float RotationSpeed = TurretPawn->GetRotationSpeed();
	float CurrentYaw = ControllingPawn->GetActorRotation().Yaw;

	float DeltaAngle = FMath::FindDeltaAngleDegrees<float>(CurrentYaw, TargetAngle);
	float ChangeInRotation = FMath::Clamp(DeltaAngle, -RotationSpeed * DeltaSeconds, RotationSpeed * DeltaSeconds);

	ControllingPawn->AddActorLocalRotation(FRotator(0.0f, ChangeInRotation, 0.0f));

	if (static_cast<int>(TargetAngle * 100) == static_cast<int>(CurrentYaw * 100))
	{
		ControllingPawn->SetActorRotation(FRotator(ControllingPawn->GetActorRotation().Pitch, TargetAngle, ControllingPawn->GetActorRotation().Roll));
	}
}
