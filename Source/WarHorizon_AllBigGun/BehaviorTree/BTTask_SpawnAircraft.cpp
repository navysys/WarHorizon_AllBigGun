// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_SpawnAircraft.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AircraftsAI.h"
#include "AIController.h"
#include "Interface/AircraftsInterface.h"

UBTTask_SpawnAircraft::UBTTask_SpawnAircraft()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_SpawnAircraft::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	bool IsSpawnEnd = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISSPAWNEND);
	if (IsSpawnEnd == false)
	{
		int MaxSpawnCount = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_MAXSPAWNCOUNT);
		int CurrentSpawnCount = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_CURRENTSPAWNCOUNT);
		AActor* MotherShipActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_MOTHERSHIPACTOR));
		if (MotherShipActor == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("AActor Cast Failed"));
			return EBTNodeResult::Failed;
		}

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

		if (MaxSpawnCount > CurrentSpawnCount)
		{
			CurrentSpawnCount++;
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_CURRENTSPAWNCOUNT, CurrentSpawnCount);
			if (MaxSpawnCount <= CurrentSpawnCount)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISSPAWNEND, true);
			}
			AircraftsPawn->SpawnAircraft(MotherShipActor->GetActorLocation());
			return EBTNodeResult::InProgress;
		}
	}
	
	return EBTNodeResult::Succeeded;
}

void UBTTask_SpawnAircraft::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Time += DeltaSeconds;
	float SpawnTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_SPAWNTIME);
	if (Time > SpawnTime)
	{
		Time = 0.0f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
