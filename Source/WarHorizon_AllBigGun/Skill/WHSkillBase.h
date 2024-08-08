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

	// ���߿� ���⼭ ��Ÿ�� ����, �ڽ�Ʈ ���濡 ���� UI ����
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
	
	// CS(���ǹ�)�� ���� �⺻ ����
	int32 AttackCount;
	int32 DefenseCount;
	int32 DamagedCount;

	// AE(�ΰ�ȿ��)�� ���� �⺻ ����
};
