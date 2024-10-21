#pragma once

#include "CoreMinimal.h"
#include "Turret/WHTurretBase.h"
#include "WHDualTurretBase.generated.h"

UENUM()
enum class DualTurretState : uint8
{
	Normal = 0,
	Air,
	Invalid
};

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHDualTurretBase : public AWHTurretBase
{
	GENERATED_BODY()
	
public:
	AWHDualTurretBase();
protected:
	virtual void Fire() override;

public:
	DualTurretState DualState = DualTurretState::Invalid;


};
