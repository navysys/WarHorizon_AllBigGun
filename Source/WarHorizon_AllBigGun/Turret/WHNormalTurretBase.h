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
	AWHNormalTurretBase();
protected:
	virtual void Fire() override;

};
