// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WHClientPlayerController.h"
#include "Widget/WHClientWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/WHClientGameMode.h"


void AWHClientPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (WidgetClass)
		{
			ClientWidget = CreateWidget<UWHClientWidgetBase>(this, WidgetClass);
			if (IsValid(ClientWidget))
			{
				ClientWidget->AddToViewport();
				SetInputMode(FInputModeUIOnly());
				SetShowMouseCursor(true);
			}
		}
	}
}

void AWHClientPlayerController::TestJoinServer(FName IP)
{
	AWHClientGameMode* ClientGameMode = Cast<AWHClientGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(ClientGameMode))
	{
		ClientGameMode->JoinServer(IP);
	}
}
