// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTDecorator_IsAttackTypeAngle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TurretAI.h"

UBTDecorator_IsAttackTypeAngle::UBTDecorator_IsAttackTypeAngle()
{
}

bool UBTDecorator_IsAttackTypeAngle::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    uint8 AttackType = BlackboardComp->GetValueAsInt(BBKEY_ATTACKTYPE);

    return AttackType == 1;
}
