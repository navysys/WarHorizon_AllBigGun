// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_SpawnAircraft.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AircraftsAI.h"
#include "AIController.h"
#include "Interface/AircraftsInterface.h"

EBTNodeResult::Type UBTTask_SpawnAircraft::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	bool bIsSpawnEnd = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISSPAWNEND);
	if (bIsSpawnEnd == false)
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
				AircraftsPawn->SetIsSpawnEnd();
			}
			AircraftsPawn->SpawnAircraft(MotherShipActor->GetActorLocation());
			return EBTNodeResult::InProgress;
		}
	}
	
	return EBTNodeResult::Succeeded;
}
