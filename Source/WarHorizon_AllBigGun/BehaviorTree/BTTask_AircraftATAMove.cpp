// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_AircraftATAMove.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/AircraftsInterface.h"
#include "BehaviorTree/AircraftsAI.h"
#include "Enum/EAircraftsState.h"
#include "Kismet\KismetSystemLibrary.h"

UBTTask_AircraftATAMove::UBTTask_AircraftATAMove()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_AircraftATAMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EAircraftsState AircraftsState = static_cast<EAircraftsState>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BBKEY_AIRCRAFTSSTATE));
	if (AircraftsState == EAircraftsState::AtA)
	{
		APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
		if (ControllingPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		IAircraftsInterface* AircraftsPawn = Cast<IAircraftsInterface>(ControllingPawn);
		if (AircraftsPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		AActor* TargetAircrafts = AircraftsPawn->GetTargetAircrafts();
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGETAIRCRAFTS, Cast<UObject>(TargetAircrafts));

		if (TargetAircrafts != nullptr)
		{
			IsHasTarget = true;
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_TARGETPOSITION, TargetAircrafts->GetActorLocation());
			return EBTNodeResult::InProgress;
		}
		else
		{
			TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
			traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // 이후 채널 수정
			FVector TargetLoc = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TARGETPOSITION);
			float Radius = 1000.0f;
			TArray<AActor*> OutActors;

			UKismetSystemLibrary::SphereOverlapActors(GetWorld(), TargetLoc, Radius, traceObjectTypes, nullptr, {}, OutActors);

			if (OutActors.Num() > 0)
			{
				OutActors.Sort([&TargetLoc](const AActor& A, const AActor& B)
					{
						return FVector::Dist(TargetLoc, A.GetActorLocation()) < FVector::Dist(TargetLoc, B.GetActorLocation());
					});
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGETAIRCRAFTS, Cast<UObject>(OutActors[0]));
				IsHasTarget = true;
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_TARGETPOSITION, TargetAircrafts->GetActorLocation());
				return EBTNodeResult::InProgress;
			}
			else
			{
				IsHasTarget = false;
				return EBTNodeResult::InProgress;
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_AircraftATAMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (IsHasTarget == true)
	{
		
		AActor* TargetAircrafts = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGETAIRCRAFTS));
		if (IsValid(TargetAircrafts))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_TARGETPOSITION, TargetAircrafts->GetActorLocation());
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGETAIRCRAFTS, nullptr);
		}
		FVector Target = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TARGETPOSITION);
		FVector Loc = ControllingPawn->GetActorLocation();
		Target.Z = 0;
		Loc.Z = 0;

		// 목표 위치에 도달하였는지 dist 로 판단
		float Dist = FVector::Dist(Target, Loc);
		float AttackRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_ATAATTACKRANGE);

		// 공격 사거리 내로 들오가면 succeeded 로 다음 task 진행
		if (Dist < AttackRange)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else
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
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}


	EAircraftsState AircraftsState = static_cast<EAircraftsState>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BBKEY_AIRCRAFTSSTATE));
	if (AircraftsState != EAircraftsState::AtA)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
