// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret/WHNormalTurretBase.h"
#include "WHSubTurretBase.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHSubTurretBase : public AWHNormalTurretBase
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;
};
