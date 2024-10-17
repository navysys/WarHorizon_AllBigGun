// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enum/ETurretType.h"
#include "Enum/ETurretAttackType.h"
#include "WHCustomStructs.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API UWHCustomStructs : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(Atomic)
struct FTurretArray
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	ETurretType TurretsType = ETurretType::Invalid;

	UPROPERTY(EditAnywhere)
	TArray<class AWHTurretBase*> Turrets;
};