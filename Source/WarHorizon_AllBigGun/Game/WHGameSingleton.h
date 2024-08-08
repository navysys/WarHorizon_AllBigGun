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

	FString GetPlayerName() { return PlayerName; }
	FString GetBattleShipName() { return BattleShipName; }
	int32 GetBattleShipInt() { return BattleShipInt; }
	uint8 GetTeamInt() { return TeamInt; }

	void SetPlayerName(FString PName) { PlayerName = PName; }
	void SetBattleShipName(FString BName) { BattleShipName = BName; }
	void SetBattleShipInt(int32 BInt) { BattleShipInt = BInt; }
	void SetTeamInt(uint8 TInt) { TeamInt = TInt; }

private:
	UDataTable* BattleShipDataTable;
	UDataTable* TurretDataTable;

	FString PlayerName = TEXT("TEST");
	FString BattleShipName = TEXT("Yamato");
	int32 BattleShipInt = 0;
	uint8 TeamInt = 1;
};
