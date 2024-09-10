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
	UDataTable* AircraftDataTable;

	FString PlayerName = TEXT("TEST");
	FString BattleShipName = TEXT("Yamato");
	int32 BattleShipInt = 0;
	uint8 TeamInt = 1;
};
