// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enum/EAircraftsType.h"
#include "AircraftDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct WARHORIZON_ALLBIGGUN_API FAircraftDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FString AircraftName = TEXT("None");
	UPROPERTY(EditAnywhere)
	EAircraftsType AircraftsType = EAircraftsType::Invalid;

	UPROPERTY(EditAnywhere)
	UStaticMesh* AircraftMesh = nullptr;

	UPROPERTY(EditAnywhere)
	float AircraftReloadTime = 9999.9999f;
	UPROPERTY(EditAnywhere)
	float AircraftRange = 9999.9999f;

	UPROPERTY(EditAnywhere)
	float HorizontalAngle = 0.0f;
	UPROPERTY(EditAnywhere)
	float VerticalAngle = 0.0f;
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 0.0f;
};
