// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WHGameMode.h"

AWHGameMode::AWHGameMode()
{
	// 헤더 최소화를 위한 레퍼런스 전달
	static ConstructorHelpers::FClassFinder<APawn> WHBattleShipBaseClassRef(TEXT("/Game/Blueprint/BP_BattleShipBase"));
	if (WHBattleShipBaseClassRef.Class)
	{
		DefaultPawnClass = WHBattleShipBaseClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/WarHorizon_AllBigGun.WHPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}