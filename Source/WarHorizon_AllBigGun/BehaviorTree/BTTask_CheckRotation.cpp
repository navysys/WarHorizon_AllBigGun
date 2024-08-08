// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_CheckRotation.h"
#include "TurretAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckRotation::UBTTask_CheckRotation()
{

}

EBTNodeResult::Type UBTTask_CheckRotation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	uint8 AttackType = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_ATTACKTYPE);
	if (AttackType == 0)
	{

	}
	else if (AttackType == 1)
	{

	}
	else if (AttackType == 2)
	{
		//포인트 공격 모드
		FVector Point = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_POINT);
		
		float PointAngle = CalculatePointAngle(ControllingPawn, Point);
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_POINTANGLE, PointAngle);

		if ((FMath::Abs(PointAngle - ControllingPawn->GetActorRotation().Yaw) <= 2.0f))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISLOOKAT, true);
			return EBTNodeResult::Succeeded;
		}
	}
	else if(AttackType == 3)
	{
		// 타겟 공격 모드
		APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
		if (nullptr == TargetPawn)
		{
			return EBTNodeResult::Failed;
		}


		float TargetAngle = CalculateTargetAngle(ControllingPawn, TargetPawn);

		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_TARGETANGLE, TargetAngle);

		if ((FMath::Abs(TargetAngle - ControllingPawn->GetActorRotation().Yaw) <= 2.0f))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISLOOKAT, true);
			return EBTNodeResult::Succeeded;
		}
	}
	

	// 최대 회전 각보다 클 경우 발사하지 못하도록 대기
	// 조건에 어긋나면 ISLOOKAT false로 변경

	return EBTNodeResult::InProgress;
}

float UBTTask_CheckRotation::CalculateTargetAngle(APawn* Start, APawn* Target)
{
	FVector TargetLotation = Target->GetActorLocation();
	FVector StartLocation = Start->GetActorLocation();

	TargetLotation.Z = 0;
	StartLocation.Z = 0;

	FVector Dir = TargetLotation - StartLocation;

	float Dot = FVector::DotProduct(FVector::ForwardVector, Dir.GetSafeNormal());
	float AcosAngle = FMath::Acos(Dot);
	float Angle = FMath::RadiansToDegrees(AcosAngle);

	FVector Cross = FVector::CrossProduct(FVector::ForwardVector, Dir.GetSafeNormal());

	if (Cross.Z > 0)
	{
		return Angle;
	}
	else if (Cross.Z < 0)
	{
		return 360-Angle;
	}

	return 0.0f;
}

float UBTTask_CheckRotation::CalculatePointAngle(APawn* Start, FVector Point)
{
	FVector StartLocation = Start->GetActorLocation();
	FVector PointLocation = Point;

	PointLocation.Z = 0;
	StartLocation.Z = 0;

	FVector Dir = PointLocation - StartLocation;

	float Dot = FVector::DotProduct(FVector::ForwardVector, Dir.GetSafeNormal());
	float AcosAngle = FMath::Acos(Dot);
	float Angle = FMath::RadiansToDegrees(AcosAngle);

	FVector Cross = FVector::CrossProduct(FVector::ForwardVector, Dir.GetSafeNormal());

	if (Cross.Z > 0)
	{
		return Angle;
	}
	else if (Cross.Z < 0)
	{
		return -Angle;
	}

	return 0;
}
