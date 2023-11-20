// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTDecorator_IsAttackTypePawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TurretAI.h"

UBTDecorator_IsAttackTypePawn::UBTDecorator_IsAttackTypePawn()
{
}

bool UBTDecorator_IsAttackTypePawn::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    uint8 AttackType = BlackboardComp->GetValueAsInt(BBKEY_ATTACKTYPE);

    return AttackType == 3;
}
