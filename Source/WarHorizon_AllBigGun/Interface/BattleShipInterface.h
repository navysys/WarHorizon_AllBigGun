// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattleShipInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBattleShipInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WARHORIZON_ALLBIGGUN_API IBattleShipInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void UserAttack() = 0;
	virtual void UserAttackCancel() = 0;
	virtual void UserSkill1() = 0;
	virtual void UserSkill2() = 0;
	virtual void UserSkill3() = 0;
	virtual void UserSkill4() = 0;
	virtual void CalculateRotationToHitPoint(FVector HitPoint) = 0;
	virtual void IncreaseMoveSpeed() = 0;
	virtual void DecreaseMoveSpeed() = 0;
};
