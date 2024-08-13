// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AircraftChangeAngle.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API UBTService_AircraftChangeAngle : public UBTService
{
	GENERATED_BODY()
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
};
