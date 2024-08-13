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
	UWHCAircraftsMovement();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	float TickDeltaTime;

	float MaxHeight;
	float MinHeight;

	float PitchRotSpeed;
	float YawRotSpeed;

public:
	void IncreaseHeight(float MaxZ);
	void DecreaseHeight(float MinZ);
	void Turn(float Angle);
};
