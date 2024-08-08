#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AircraftsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAircraftsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class WARHORIZON_ALLBIGGUN_API IAircraftsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual TArray<AActor*> GetArrayAircrafts() = 0;
	virtual uint8 GetAircraftType() = 0;
	virtual void AddAircraft(FVector StartPos) = 0;

	virtual void Fire() = 0;
};