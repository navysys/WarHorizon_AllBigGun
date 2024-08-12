// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enum/EAircraftType.h"
#include "AircraftDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct WARHORIZON_ALLBIGGUN_API FAircraftDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FString AircraftName;
	UPROPERTY(EditAnywhere)
	EAircraftType AircraftType;

	UPROPERTY(EditAnywhere)
	UStaticMesh* AircraftMesh;

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
