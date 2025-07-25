#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattleShipInterface.generated.h"


UINTERFACE(MinimalAPI)
class UBattleShipInterface : public UInterface
{
	GENERATED_BODY()
};


class WARHORIZON_ALLBIGGUN_API IBattleShipInterface
{
	GENERATED_BODY()

public:
	virtual void RapidAttack() = 0;
	virtual void NormalAttack() = 0;
	virtual void SpinTurrets(AActor* Target) = 0;
	virtual void SpinTurrets(FVector HitPoint) = 0;
	virtual void SpinBattleShip(FVector HitPoint) = 0;
	virtual void IncreaseMoveSpeed() = 0;
	virtual void DecreaseMoveSpeed() = 0;
	virtual void SpawnAircrafts(int Index) = 0;
};
