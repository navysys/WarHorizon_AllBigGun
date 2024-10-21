#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enum/ETurretType.h"
#include "WHCustomStructs.generated.h"


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

USTRUCT(Atomic)
struct FTargetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Angle = 0;

	UPROPERTY(EditAnywhere)
	float Distance = 0;
};