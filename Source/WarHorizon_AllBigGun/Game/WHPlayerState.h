// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Enum/EBattleShipType.h"
#include "WHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	// Stat
protected:
	// UI 변경은 state 변수 기준
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	FString BattleShipName;

	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	EBattleShipType BattleShipType = EBattleShipType::Invalid;

	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	uint8 TeamInt = 1;


	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float InitMaxHP;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float HP;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float InitMaxMP;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float MP;



};
