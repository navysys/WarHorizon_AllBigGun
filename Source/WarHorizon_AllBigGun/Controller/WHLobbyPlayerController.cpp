// Fill out your copyright notice in the Description page of Project Settings.


#include "WarHorizon_AllBigGun/Controller/WHLobbyPlayerController.h"
#include "WarHorizon_AllBigGun/Widget/WHLobbyWidgetBase.h"
#include "WarHorizon_AllBigGun/Game/WHLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"

void AWHLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (WidgetClass)
		{
			LobbyWidget = CreateWidget<UWHLobbyWidgetBase>(this, WidgetClass);
			if (IsValid(LobbyWidget))
			{
				LobbyWidget->AddToViewport();
				SetInputMode(FInputModeUIOnly());
				SetShowMouseCursor(true);
			}
		}
	}
}

void AWHLobbyPlayerController::TestJoinServer(FName IP)
{
	AWHLobbyGameMode* ClientGameMode = Cast<AWHLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(ClientGameMode))
	{
		ClientGameMode->JoinServer(IP);
	}
}
