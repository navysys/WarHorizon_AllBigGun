
#include "Skill/WHSkillBase.h"
#include "BattleShip/WHBattleShipBase.h"

UWHSkillBase::UWHSkillBase()
{

}

void UWHSkillBase::Init(AWHBattleShipBase* Base)
{
	BaseBattleShip = Base;
}

void UWHSkillBase::Effect()
{
	if (!IsValid(BaseBattleShip))
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill : BaseBattleShip Not Valid"));
		return;
	}
}
