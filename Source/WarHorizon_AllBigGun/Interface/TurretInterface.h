// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TurretInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTurretInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WARHORIZON_ALLBIGGUN_API ITurretInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual APawn* GetBaseBattleShip() = 0;
	virtual uint8 GetAttackType() = 0;
	virtual float GetTurretTargetAngle() = 0;
	virtual APawn* GetTurretTargetPawn() = 0;
	virtual FVector GetTurretTargetPoint() = 0;
	virtual float GetRotationSpeed() = 0;
	virtual float GetReloadTime() = 0;
	virtual float GetMaxHorizontalAngle() = 0;

	virtual void Fire() = 0;
};
