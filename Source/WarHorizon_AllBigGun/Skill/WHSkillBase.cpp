
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
	// �����Ƽ���� �ڽ�Ʈ ��Ÿ�� ��� �ѱ�� ���� effect ��� ���� �� ��ų���� ��� ����
	// �� ���� ���� �� ��ų ��Ÿ�� ���� �� ���� ���Ǻγ� �޼� ȿ���鿡 ���� ���� �̸� ���� �� �����ϴ� �������� ����
}
