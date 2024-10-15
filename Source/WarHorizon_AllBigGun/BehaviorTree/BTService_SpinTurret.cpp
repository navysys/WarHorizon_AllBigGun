// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTService_SpinTurret.h"
#include "TurretAI.h"
#include "Interface/TurretInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"

UBTService_SpinTurret::UBTService_SpinTurret()
{
}

void UBTService_SpinTurret::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	ITurretInterface* TurretPawn = Cast<ITurretInterface>(ControllingPawn);
	if (nullptr == TurretPawn)
	{
		return;
	}

	float MaxAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_MAXANGLE);
	float MaxYaw = MaxAngle / 2;
	float MinYaw = -MaxAngle / 2;

	float InitBattleShipAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_BATTLESHIPANGLE);
	float CurrentBattleShipAngle = 0.0f; //TurretPawn->GetBaseBattleShip()->GetActorRotation().Yaw;
	float BattleShipAngle = CurrentBattleShipAngle - InitBattleShipAngle;

	float InitRotationYaw = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_INITROTATION);

	float RotationSpeed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_ROTATIONSPEED);

	float CurrentYaw = ControllingPawn->GetActorRotation().Yaw;
	if (CurrentYaw < 0)
	{
		CurrentYaw += 360;
	}
	if(CurrentYaw > 360)
	{
		CurrentYaw -= 360;
	}

	float LocalYaw = CurrentYaw - InitRotationYaw - BattleShipAngle;
	if (LocalYaw > 360)
	{
		LocalYaw -= 360;
	}
	else if (LocalYaw < -360)
	{
		LocalYaw += 360;
	}

	if (LocalYaw < -180)
	{
		LocalYaw += 360;
	}
	else if (LocalYaw >= 180)
	{
		LocalYaw -= 360;
	}


	float ParentYaw = 0.0f; //TurretPawn->GetBaseBattleShip()->GetActorRotation().Yaw;
	if (ParentYaw < 0)
	{
		ParentYaw += 360;
	}

	uint8 AttackType = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_ATTACKTYPE);
	if (AttackType == 0)
	{
		return;
	}
	else if (AttackType == 1)
	{
		// 주포, 어뢰용 마우스 위치와 함선의 각도를 함선으로부터 전달 받아 방향 전환
		float MouseAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_MOUSEANGLE);

		float LocalGoalAngle = 0;
		if (MouseAngle != 0)
		{
			LocalGoalAngle = MouseAngle - InitRotationYaw - BattleShipAngle;
			if (LocalGoalAngle >= 180)
			{
				LocalGoalAngle -= 360;
			}
			else if (LocalGoalAngle < -180)
			{
				LocalGoalAngle += 360;
			}
		}

		float GoalAngle = 0;
		if (MouseAngle != 0)
		{
			GoalAngle = LocalGoalAngle - LocalYaw;
			if (GoalAngle >= 180)
			{
				GoalAngle -= 360;
			}
			else if (GoalAngle < -180)
			{
				GoalAngle += 360;
			}
		}

		// 로컬 회전 목표 값이 0보다 크고 MaxYaw 보다 작을 경우
		if (LocalGoalAngle > 0 && LocalGoalAngle <= MaxYaw)
		{
			// 포탑의 로컬 회전값이 음수일 경우 (반대 방향으로 회전된 포탑을 정방향으로 전환)
			if (LocalYaw < 0)
			{
				ControllingPawn->AddActorLocalRotation(FRotator(0.0f, RotationSpeed * 3 * DeltaSeconds, 0.0f));
			}
			// 포탑이 반대 방향이 아니고 정방향으로 더 회전해야 할 경우
			else if (GoalAngle > 0)
			{
				// 포탑의 로컬 회전 값이 MaxYaw 보다 작을 경우 (정상적인 회전)
				if (LocalYaw < MaxYaw)
				{
					ControllingPawn->AddActorLocalRotation(FRotator(0.0f, RotationSpeed * 3 * DeltaSeconds, 0.0f));
				}
				// 포탑이 회전해야 할 값을 초과한 경우
				else if (LocalYaw > MaxYaw)
				{
					ControllingPawn->SetActorRotation(FRotator(0, InitRotationYaw + MaxYaw + BattleShipAngle, 0));
				}
			}
			// 포탑이 반대 방향은 아니지만 역방향으로 회전해야 할 경우
			else if (GoalAngle < 0)
			{
				ControllingPawn->AddActorLocalRotation(FRotator(0.0f, -RotationSpeed * 3 * DeltaSeconds, 0.0f));
			}
		}
		// 로컬 회전 값이 0보다 크면서 MaxYaw 보다 큰 경우 (포탑 회전이 불가능한 위치) - 위의 코드와 동작 원리는 동일
		else if (LocalGoalAngle > 0 && LocalGoalAngle > MaxYaw)
		{
			if (LocalYaw < 0)
			{
				ControllingPawn->AddActorLocalRotation(FRotator(0.0f, RotationSpeed * 3 * DeltaSeconds, 0.0f));
			}
			else if (GoalAngle > 0)
			{
				if (LocalYaw < MaxYaw)
				{
					ControllingPawn->AddActorLocalRotation(FRotator(0.0f, RotationSpeed * 3 * DeltaSeconds, 0.0f));
				}
				else if (LocalYaw > MaxYaw)
				{
					ControllingPawn->SetActorRotation(FRotator(0, InitRotationYaw + MaxYaw + BattleShipAngle, 0));
				}
			}
		}
		// 로컬 회전 값이 0보다 작으면서 MinYaw 보다 큰 경우
		else if (LocalGoalAngle < 0 && LocalGoalAngle >= MinYaw)
		{
			// 포탑의 로컬 회전값이 양수일 경우 (반대 방향으로 회전된 포탑을 정방향으로 전환)
			if (LocalYaw > 0)
			{
				ControllingPawn->AddActorLocalRotation(FRotator(0.0f, - RotationSpeed * 3 * DeltaSeconds, 0.0f));
			}
			// 포탑이 반대 방향이 아니고 정방향으로 더 회전해야 할 경우
			else if (GoalAngle < 0)
			{
				// 포탑의 로컬 회전 값이 MinYaw 보다 작을 경우 (정상적인 회전)
				if (LocalYaw > MinYaw)
				{
					ControllingPawn->AddActorLocalRotation(FRotator(0.0f, -RotationSpeed * 3 * DeltaSeconds, 0.0f));
				}
				// 포탑이 회전해야 할 값을 초과한 경우
				else if (LocalYaw < MinYaw)
				{
					ControllingPawn->SetActorRotation(FRotator(0, InitRotationYaw + MinYaw + BattleShipAngle, 0));
				}
			}
			// 포탑이 반대 방향은 아니지만 역방향으로 회전해야 할 경우
			else if (GoalAngle > 0)
			{
				ControllingPawn->AddActorLocalRotation(FRotator(0.0f, RotationSpeed * 3 * DeltaSeconds, 0.0f));
			}
		}
		// 로컬 회전 값이 0보다 작으면서 MinYaw 보다 작은 경우 (포탑 회전이 불가능한 위치) - 위의 코드와 동작 원리는 동일
		else if (LocalGoalAngle < 0 && LocalGoalAngle < MinYaw)
		{
			if (LocalYaw > 0)
			{
				ControllingPawn->AddActorLocalRotation(FRotator(0.0f, -RotationSpeed * 3 * DeltaSeconds, 0.0f));
			}
			else if (GoalAngle < 0)
			{
				if (LocalYaw > MinYaw)
				{
					ControllingPawn->AddActorLocalRotation(FRotator(0.0f, -RotationSpeed * 3 * DeltaSeconds, 0.0f));
				}
				else if (LocalYaw < MinYaw)
				{
					ControllingPawn->SetActorRotation(FRotator(0, InitRotationYaw + MinYaw + BattleShipAngle, 0));
				}
			}
		}
		
		if (static_cast<int>(GoalAngle * 10) == 0 && MouseAngle != 0)
		{
			
			ControllingPawn->SetActorRotation(FRotator(0, MouseAngle , 0));
		}

	}
	else if (AttackType == 2)
	{
		// 포인트 공격 모드 - 특정 지점에 지정한 포들이 뭉쳐서 공격하는 것을 구현하기 위한 모드
		float PointAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_POINTANGLE);
		float DeltaAngle = FMath::FindDeltaAngleDegrees<float>(CurrentYaw, PointAngle);
		float ChangeInRotation = FMath::Clamp(DeltaAngle, -RotationSpeed * DeltaSeconds, RotationSpeed * DeltaSeconds);

		// 로컬 좌표 기준으로 회전 제한
		if (CurrentYaw - InitRotationYaw - ParentYaw < MaxYaw && CurrentYaw - InitRotationYaw - ParentYaw < MinYaw)
		{
			ControllingPawn->AddActorLocalRotation(FRotator(0.0f, ChangeInRotation, 0.0f));
		}

		if (static_cast<int>(PointAngle * 100) == static_cast<int>(CurrentYaw * 100))
		{
			ControllingPawn->SetActorRotation(FRotator(ControllingPawn->GetActorRotation().Pitch, PointAngle, ControllingPawn->GetActorRotation().Roll));
		}
	}
	else if(AttackType == 3)
	{
		// 타겟 공격 모드 - 자동으로 범위 내의 적을 공격하기 위한 모드로 차후 리드샷 거리 자동 계산 추가해야 함
		float TargetAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_TARGETANGLE);
		float DeltaAngle = FMath::FindDeltaAngleDegrees<float>(CurrentYaw, TargetAngle);;
		float ChangeInRotation = FMath::Clamp(DeltaAngle, -RotationSpeed * DeltaSeconds, RotationSpeed * DeltaSeconds);

		// 로컬 좌표 기준으로 회전 제한
		if (CurrentYaw - InitRotationYaw - ParentYaw < MaxYaw && CurrentYaw - InitRotationYaw - ParentYaw > MinYaw)
		{
			ControllingPawn->AddActorLocalRotation(FRotator(0.0f, ChangeInRotation, 0.0f));
		}


		if (static_cast<int>(TargetAngle * 100) == static_cast<int>(CurrentYaw * 100))
		{
			ControllingPawn->SetActorRotation(FRotator(0, TargetAngle, 0));
		}
	}
}
