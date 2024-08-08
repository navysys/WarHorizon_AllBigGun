// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "WHCAircraftsMovement.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API UWHCAircraftsMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	float CurrentHeight = 6000.0f;
	float MaxHeight;
	float MinHeight;

	//float MaxAscentSpeed = 600.0f;
	float AscentSpeed = 1000;
	//float MaxDescentSpeed = 600.0f;
	float DescentSpeed = 1000;

	//float AscentAcceleration = 120.0f;

public:
	void IncreaseHeight(float DeltaTime, FVector Location);
	void DecreaseHeight(float DeltaTime, FVector Location);
};
