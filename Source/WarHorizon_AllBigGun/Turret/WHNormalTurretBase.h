// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret/WHTurretBase.h"
#include "WHNormalTurretBase.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHNormalTurretBase : public AWHTurretBase
{
	GENERATED_BODY()
	
public:
	virtual void Fire() override;

public:



};
