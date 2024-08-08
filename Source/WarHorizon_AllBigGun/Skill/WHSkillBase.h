// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WHSkillBase.generated.h"

/**
 * 
 */
class AWHBattleShip;

UCLASS(BlueprintType)
class WARHORIZON_ALLBIGGUN_API UWHSkillBase : public UObject
{
	GENERATED_BODY()
	
public:
	UWHSkillBase();
	UWHSkillBase(AWHBattleShip* BaseShip);

	// 나중에 여기서 쿨타임 적용, 코스트 변경에 따른 UI 변경
	virtual void Effect();

protected:
	bool CSAttackHit(int Num);
	bool CSDefenseShield(int Num);
	bool CSTakeDamage(int Num);

	void AECoolDownSelf(float Time);
	void AECoolDownAll(float Time);

private:
	AWHBattleShip* BaseBattleShip;

public:
	float Value;
	float Coefficient;
	float Cost;
	float CoolTime;
	
	// CS(조건문)를 위한 기본 변수
	int32 AttackCount;
	int32 DefenseCount;
	int32 DamagedCount;

	// AE(부가효과)를 위한 기본 변수
};
