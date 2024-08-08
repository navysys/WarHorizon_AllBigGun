// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WHGameMode.h"

AWHGameMode::AWHGameMode()
{
	// 헤더 최소화를 위한 레퍼런스 전달
	static ConstructorHelpers::FClassFinder<APawn> WHCameraPawnClassRef(TEXT("/Game/Blueprint/BP_CameraPawn"));
	if (WHCameraPawnClassRef.Class)
	{
		DefaultPawnClass = WHCameraPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/WarHorizon_AllBigGun.WHPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}