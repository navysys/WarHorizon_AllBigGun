// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataTable/BattleShipDataTable.h"
#include "DataTable/TurretDataTable.h"
#include "WHGameSingleton.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogBattleShipSingleton, Error, All);

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API UWHGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UWHGameSingleton();
	static UWHGameSingleton& Get();

public:
	FORCEINLINE UDataTable* GetBattleShipDataTable() { return BattleShipDataTable; }
	FORCEINLINE UDataTable* GetTurretDataTable() { return TurretDataTable; }

private:
	UDataTable* BattleShipDataTable;
	UDataTable* TurretDataTable;
};
