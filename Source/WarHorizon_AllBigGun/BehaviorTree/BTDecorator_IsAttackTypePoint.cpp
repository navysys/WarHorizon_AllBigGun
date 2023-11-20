
// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTDecorator_IsAttackTypePoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TurretAI.h"

UBTDecorator_IsAttackTypePoint::UBTDecorator_IsAttackTypePoint()
{
}

bool UBTDecorator_IsAttackTypePoint::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    uint8 AttackType = BlackboardComp->GetValueAsInt(BBKEY_ATTACKTYPE);

    return AttackType == 2;
}
