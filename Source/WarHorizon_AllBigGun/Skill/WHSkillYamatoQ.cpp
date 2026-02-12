// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/WHSkillYamatoQ.h"
#include "BattleShip/WHBattleShipBase.h"

bool UWHSkillYamatoQ::Effect()
{
	if (Super::Effect())
	{
		AWHBattleShipBase* BaseShip = Cast<AWHBattleShipBase>(BaseUnit);
		if (IsValid(BaseShip))
		{
			if (BaseShip->NormalAttack(CoolTime))
			{
				// 정상적으로 공격이 성공했을 때

				return true;
			}
		}
	}
	return false;
}
