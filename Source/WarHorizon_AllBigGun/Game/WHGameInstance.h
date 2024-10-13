// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DataTable/BattleShipDataTable.h"
#include "DataTable/TurretDataTable.h"
#include "DataTable/AircraftDataTable.h"
#include "WHGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API UWHGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UWHGameInstance();

public:
	FORCEINLINE UDataTable* GetBattleShipDataTable() { return BattleShipDataTable; }
	FORCEINLINE UDataTable* GetTurretDataTable() { return TurretDataTable; }
	FORCEINLINE UDataTable* GetAircraftDataTable() { return AircraftDataTable; }

private:
	UDataTable* BattleShipDataTable;
	UDataTable* TurretDataTable;
	UDataTable* AircraftDataTable;
};
