// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WHTurretAIController.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHTurretAIController : public AAIController
{
	GENERATED_BODY()

public:
	AWHTurretAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
