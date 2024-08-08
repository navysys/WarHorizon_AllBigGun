// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enum/ETurretType.h"
#include "TurretDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct WARHORIZON_ALLBIGGUN_API FTurretDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	uint16 TurretID;
	UPROPERTY(EditAnywhere)
	FString TurretName;
	UPROPERTY(EditAnywhere)
	ETurretType TurretType;

	UPROPERTY(EditAnywhere)
	UStaticMesh* TurretMesh;

	UPROPERTY(EditAnywhere)
	float ReloadTime;
	UPROPERTY(EditAnywhere)
	float Range;

	UPROPERTY(EditAnywhere)
	float HorizontalAngle;
	UPROPERTY(EditAnywhere)
	float VerticalAngle;
	UPROPERTY(EditAnywhere)
	float RotationSpeed;
};
