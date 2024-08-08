// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_GetAttackType.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/TurretInterface.h"
#include "AIController.h"
#include "TurretAI.h"

UBTTask_GetAttackType::UBTTask_GetAttackType()
{
}

EBTNodeResult::Type UBTTask_GetAttackType::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	ITurretInterface* TurretPawn = Cast<ITurretInterface>(ControllingPawn);
	if (nullptr == TurretPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 초기 값이라면 터렛의 값을 받아옴
	uint8 BBAttackType = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_ATTACKTYPE);
	if (BBAttackType == 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_ATTACKTYPE, TurretPawn->GetAttackType());
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_RELOADTIME, TurretPawn->GetReloadTime());
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_MAXANGLE, TurretPawn->GetMaxHorizontalAngle());
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_ROTATIONSPEED, TurretPawn->GetRotationSpeed());
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_BATTLESHIPANGLE, TurretPawn->GetBaseBattleShip()->GetActorRotation().Yaw);

		float InitRotationYaw = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_INITROTATION);
		if (InitRotationYaw == 9999.9f)
		{
			InitRotationYaw = ControllingPawn->GetActorRotation().Yaw;
			if (InitRotationYaw < 0)
			{
				InitRotationYaw += 360;
			}
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_INITROTATION, InitRotationYaw);
		}
	}
	uint8 AttackType = TurretPawn->GetAttackType();
	if (BBAttackType != AttackType)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_ATTACKTYPE, AttackType);
		if (AttackType == 1)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_TARGETANGLE, TurretPawn->GetTurretTargetAngle());
		}
		else if (AttackType == 2)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_POINTANGLE, TurretPawn->GetTurretTargetPoint());
		}
		else if (AttackType == 3)
		{
			// 블랙보드 타겟은 이후 자동으로 찾게 되어 있기 때문에 설정 X
			// 다른 추가 리드샷 값을 지정한다면 여기서 초기화 해주면 될 것
		}
		// 컨트롤러는 그대로 두고 포탑만 교체한 상황을 가정
		else if (AttackType == 0)
		{

		}
	}
	else
	{
		if (AttackType == 1)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_MOUSEANGLE, TurretPawn->GetTurretTargetAngle());
		}
		else if (AttackType == 2)
		{

		}
		else if (AttackType == 3)
		{

		}
	}
	

	return EBTNodeResult::Succeeded;
}
