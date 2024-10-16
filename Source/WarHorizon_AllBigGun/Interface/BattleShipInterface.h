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
	virtual void RapidAttack() = 0;
	virtual void NormalAttack() = 0;
	virtual void SpinTurrets(AActor* Target) = 0;
	virtual void SpinTurrets(FVector HitPoint) = 0;
	virtual void UseSkill(char Key) = 0;
	virtual void SpinBattleShip(FVector HitPoint) = 0;
	virtual void IncreaseMoveSpeed() = 0;
	virtual void DecreaseMoveSpeed() = 0;
	virtual void SpawnAircrafts(int Index) = 0;
};
