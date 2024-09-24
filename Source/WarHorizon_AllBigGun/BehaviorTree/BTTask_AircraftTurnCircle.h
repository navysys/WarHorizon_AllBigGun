// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AircraftTurnCircle.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API UBTTask_AircraftTurnCircle : public UBTTaskNode
{
	GENERATED_BODY()
	
	APawn* ControllingPawn;

public:
	UBTTask_AircraftTurnCircle();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
