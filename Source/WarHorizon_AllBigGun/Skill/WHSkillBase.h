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
	virtual bool Effect();

public:
	ISkillUsableInterface* BaseUnit;

	UPROPERTY(EditAnywhere)
	float Value;
	UPROPERTY(EditAnywhere)
	float Coefficient;
	UPROPERTY(EditAnywhere)
	float Cost;
	UPROPERTY(EditAnywhere)
	float CoolTime = 1.0f;

	// 이펙트도 여기서 소유해야 할 것
};
