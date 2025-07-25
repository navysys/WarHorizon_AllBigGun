#pragma once

#include "CoreMinimal.h"
#include "Interface/SkillUsableInterface.h"
#include "WHSkillBase.generated.h"


UCLASS(Blueprintable)
class WARHORIZON_ALLBIGGUN_API UWHSkillBase : public UObject
{
	GENERATED_BODY()
	
public:
	UWHSkillBase();

	void Init(ISkillUsableInterface* Base);
	virtual void Effect();

public:
	ISkillUsableInterface* BaseUnit;

	float Value;
	float Coefficient;
	float Cost;
	float CoolTime;

	// ����Ʈ�� ���⼭ �����ؾ� �� ��
};
