// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WHClientGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHClientGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void JoinServer(FName ServerIP);
};
