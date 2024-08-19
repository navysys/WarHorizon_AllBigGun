// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTService_AircraftChangeAngle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/AircraftsInterface.h"
#include "BehaviorTree/AircraftsAI.h"
#include "Enum/EAircraftsState.h"

void UBTService_AircraftChangeAngle::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}

	IAircraftsInterface* AircraftsPawn = Cast<IAircraftsInterface>(ControllingPawn);
	if (AircraftsPawn == nullptr)
	{
		return;
	}

	EAircraftsState AircraftsState = static_cast<EAircraftsState>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BBKEY_AIRCRAFTSSTATE));

	
	
	
	
	
	



	switch (AircraftsState)
	{
		case EAircraftsState::MoveStraight:
		{
			FVector Target = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TARGETPOSITION);
			FVector Loc = ControllingPawn->GetActorLocation();
			Target.Z = 0;
			Loc.Z = 0;
			FVector Dir = (Target - Loc).GetSafeNormal2D();
			int Angle = ControllingPawn->GetActorRotation().Yaw - Dir.Rotation().Yaw;

			// ���ϴ� ��������� ȸ���ϰ� ���� ����
			if (Angle > 180)
			{
				Angle -= 360;
			}
			else if (Angle < -180)
			{
				Angle += 360;
			}

			if (Angle > -1 && Angle < 1)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISSTRAIGHTREADY, true);
			}

			bool IsStraightReady = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISSTRAIGHTREADY);
			if (IsStraightReady == false)
			{
				AircraftsPawn->Turn(Angle);
			}
			break;
		}

		case EAircraftsState::MoveNormal:
		{
			FVector Target = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TARGETPOSITION);
			FVector Loc = ControllingPawn->GetActorLocation();
			Target.Z = 0;
			Loc.Z = 0;
			FVector Dir = (Target - Loc).GetSafeNormal2D();
			int Angle = ControllingPawn->GetActorRotation().Yaw - Dir.Rotation().Yaw;

			// ��ǥ ��ġ�� �����Ͽ����� dist �� �Ǵ�
			float Dist = FVector::Dist(Target, Loc);
			// IsReachTargetPosition �� false ��ȯ�� WaitState Task���
			if (Dist < 100.0f)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(BBKEY_AIRCRAFTSSTATE, static_cast<uint8>(EAircraftsState::Wait));
				return;
			}

			if (Angle > 180)
			{
				Angle -= 360;
			}
			else if (Angle < -180)
			{
				Angle += 360;
			}
			AircraftsPawn->Turn(Angle);
			break;
		}

		case EAircraftsState::Wait:
		{
			float WaitAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_WAITANGLE);
			FVector Center = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_WAITINGPOSITION); // ���� �߽�
			FVector CurrentLocation = ControllingPawn->GetActorLocation();
			float RotationSpeed = 30.0f; // �ʱ� ȸ�� �ӵ� (�� ����)


			WaitAngle += RotationSpeed * DeltaSeconds;
			if (WaitAngle >= 180.0f)
			{
				RotationSpeed = 15.0f;	// ���� �ӵ� 1500 ���� 15���� ȸ���ϸ� �������� ���ڸ� ȸ��
			}

			FRotator Rot = ControllingPawn->GetActorRotation();
			Rot.Yaw += RotationSpeed * DeltaSeconds;

			ControllingPawn->SetActorRotation(Rot);
			break;
		}


		case EAircraftsState::AtA:
		{
			break;
		}

		case EAircraftsState::AtB:
		{
			break;
		}

		case EAircraftsState::Invalid:
		{
			break;
		}
		
		default:
		{
			break;
		}
	}
}
