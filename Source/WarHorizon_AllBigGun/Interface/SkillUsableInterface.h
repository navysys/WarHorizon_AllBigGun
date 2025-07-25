#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillUsableInterface.generated.h"


UINTERFACE(MinimalAPI)
class USkillUsableInterface : public UInterface
{
	GENERATED_BODY()
};

class WARHORIZON_ALLBIGGUN_API ISkillUsableInterface
{
	GENERATED_BODY()

public:
	virtual UObject* GetSkill(char Button) = 0;
	virtual bool CanUseSkill(char Code) = 0;

};