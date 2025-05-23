#pragma once

#include "CoreMinimal.h"
#include "BattleShip/WHBattleShipBase.h"
#include "WHSkillBase.generated.h"


UCLASS(Blueprintable)
class WARHORIZON_ALLBIGGUN_API UWHSkillBase : public UObject
{
	GENERATED_BODY()
	
public:
	UWHSkillBase();

	void Init(AWHBattleShipBase* Base);
	virtual void Effect();

public:
	AWHBattleShipBase* BaseBattleShip;

	float Value;
	float Coefficient;
	float Cost;
	float CoolTime;
};
