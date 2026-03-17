
#include "WarHorizon_AllBigGun/Skill/WHSkillBase.h"
#include "WarHorizon_AllBigGun/BattleShip/WHBattleShipBase.h"

UWHSkillBase::UWHSkillBase()
{

}

void UWHSkillBase::Init(ISkillUsableInterface* Base)
{
	BaseUnit = Base;
}

bool UWHSkillBase::Effect()
{
	if (BaseUnit == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill : BaseUnit Not Valid"));
		return false;
	}
	return true;
}
