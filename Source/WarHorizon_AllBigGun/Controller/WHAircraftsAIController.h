// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/AircraftsInterface.h"
#include "WHAircraftsAIController.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHAircraftsAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AWHAircraftsAIController();

	void RunAI();
	void StopAI();

	virtual void BeginPlay() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	APawn* ControllingPawn;

	IAircraftsInterface* AircraftsPawn;

	UPROPERTY(VisibleAnywhere)
	APawn* MotherShipPawn;
	UPROPERTY(VisibleAnywhere)
	int MaxAircraftSpawnCount;
	//UPROPERTY(VisibleAnywhere)
	//int MaxAircraftSpawnCount;
};
