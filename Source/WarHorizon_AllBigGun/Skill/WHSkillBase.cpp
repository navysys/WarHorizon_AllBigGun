// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/WHSkillBase.h"
#include "BattleShip/WHBattleShip.h"

UWHSkillBase::UWHSkillBase()
{
	BaseBattleShip = nullptr;
}

UWHSkillBase::UWHSkillBase(AWHBattleShip* BaseShip)
{
	BaseBattleShip = BaseShip;
}

void UWHSkillBase::Effect()
{
	// �����Ƽ���� �ڽ�Ʈ ��Ÿ�� ��� �ѱ�� ���� effect ��� ���� �� ��ų���� ��� ����
	// �� ���� ���� �� ��ų ��Ÿ�� ���� �� ���� ���Ǻγ� �޼� ȿ���鿡 ���� ���� �̸� ���� �� �����ϴ� �������� ����
}
