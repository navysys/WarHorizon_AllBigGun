// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WHGameMode.h"

AWHGameMode::AWHGameMode()
{
	// 헤더 최소화를 위한 레퍼런스 전달
	static ConstructorHelpers::FClassFinder<APawn> BattleShipClassRef(TEXT("/Script/WarHorizon_AllBigGun.WHBattleShip"));
	if (BattleShipClassRef.Class)
	{
		DefaultPawnClass = BattleShipClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/WarHorizon_AllBigGun.WHPlayerController"));
	if (BattleShipClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}