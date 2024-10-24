// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WHClientGameMode.h"
#include "Kismet/GameplayStatics.h"

void AWHClientGameMode::JoinServer(FName ServerIP)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(ServerIP));
}
