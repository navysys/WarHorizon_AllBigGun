
#include "Skill/WHSkillBase.h"
#include "BattleShip/WHBattleShipBase.h"

UWHSkillBase::UWHSkillBase()
{
	BaseBattleShip = nullptr;
}

UWHSkillBase::UWHSkillBase(AWHBattleShipBase* BaseShip)
{
	BaseBattleShip = BaseShip;
}

void UWHSkillBase::Effect()
{
	// 어빌리티에서 코스트 쿨타임 계산 넘기고 이후 effect 상속 받은 각 스킬들이 기능 구현
	// 단 주포 명중 시 스킬 쿨타임 감소 와 같이 조건부나 달성 효과들에 대해 전부 미리 제작 후 조립하는 과정으로 제작
}
