// Fill out your copyright notice in the Description page of Project Settings.


#include "WarHorizon_AllBigGun/Game/WHLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"

void AWHLobbyGameMode::JoinServer(FName ServerIP)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->ClientTravel(TEXT("127.0.0.1:7777"), TRAVEL_Absolute);
	}
}
