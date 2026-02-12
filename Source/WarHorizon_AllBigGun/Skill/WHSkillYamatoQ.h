// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/WHSkillBase.h"
#include "WHSkillYamatoQ.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API UWHSkillYamatoQ : public UWHSkillBase
{
	GENERATED_BODY()
	
public:
	virtual bool Effect() override;
};
