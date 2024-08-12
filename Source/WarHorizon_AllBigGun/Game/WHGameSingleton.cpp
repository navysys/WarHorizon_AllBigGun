// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WHGameSingleton.h"

DEFINE_LOG_CATEGORY(LogBattleShipSingleton);

UWHGameSingleton::UWHGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> BattleShipDataRef(TEXT("/Game/DataTable/DT_BattleShipData"));
	if (nullptr != BattleShipDataRef.Object)
	{
		BattleShipDataTable = BattleShipDataRef.Object;
		check(BattleShipDataTable->GetRowMap().Num() > 0);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> TurretDataRef(TEXT("/Game/DataTable/DT_TurretData"));
	if (nullptr != TurretDataRef.Object)
	{
		TurretDataTable = TurretDataRef.Object;
		check(TurretDataTable->GetRowMap().Num() > 0);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> AircraftDataRef(TEXT("/Game/DataTable/DT_AircraftData"));
	if (nullptr != AircraftDataRef.Object)
	{
		AircraftDataTable = AircraftDataRef.Object;
		check(AircraftDataTable->GetRowMap().Num() > 0);
	}
}

UWHGameSingleton& UWHGameSingleton::Get()
{
	UWHGameSingleton* Singleton = CastChecked<UWHGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogBattleShipSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UWHGameSingleton>();
}