#pragma once

#include "CoreMinimal.h"
#include "Turret/WHTurretBase.h"
#include "WHAirTurretBase.generated.h"


class AWHBullet;


UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHAirTurretBase : public AWHTurretBase
{
	GENERATED_BODY()
	
public:
	AWHAirTurretBase();

	virtual void Fire() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWHBullet> Bullet;

	UPROPERTY(EditAnywhere)
	float HorizontalAngle = 70.0f;
};
