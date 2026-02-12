// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WHGameState.generated.h"


UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

};
