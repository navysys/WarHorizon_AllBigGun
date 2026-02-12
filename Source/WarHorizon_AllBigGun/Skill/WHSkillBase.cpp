
#include "Skill/WHSkillBase.h"
#include "BattleShip/WHBattleShipBase.h"

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
