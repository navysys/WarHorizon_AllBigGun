#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enum/ETurretType.h"
#include "TurretDataTable.generated.h"


USTRUCT(BlueprintType)
struct WARHORIZON_ALLBIGGUN_API FTurretDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	uint16 TurretID = 0.0f;
	UPROPERTY(EditAnywhere)
	FString TurretName = TEXT("None");
	UPROPERTY(EditAnywhere)
	ETurretType TurretType = ETurretType::Invalid;

	UPROPERTY(EditAnywhere)
	UStaticMesh* TurretMesh = nullptr;

	UPROPERTY(EditAnywhere)
	float TurretReloadTime = 9999.9999f;
	UPROPERTY(EditAnywhere)
	float TurretRange = 9999.9999f;

	UPROPERTY(EditAnywhere)
	float TurretHorizontalAngle = 0.0f;
	UPROPERTY(EditAnywhere)
	float TurretVerticalAngle = 0.0f;
	UPROPERTY(EditAnywhere)
	float TurretRotationSpeed = 0.0f;
};
