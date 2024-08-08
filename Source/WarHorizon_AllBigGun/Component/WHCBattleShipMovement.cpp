// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WHCBattleShipMovement.h"


UWHCBattleShipMovement::UWHCBattleShipMovement()
{
	PawnOwner = Cast<APawn>(GetOwner());
}

void UWHCBattleShipMovement::BeginPlay()
{
	Super::BeginPlay();

	MaxSpeed = 0;
	TurningBoost = 1;
}


void UWHCBattleShipMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentSpeed = Velocity.Size();
	NormalizedSpeed = CurrentSpeed / InitMaxSpeed;

	if (bCanMove)
	{
		if (bIsReverseDirection)
		{
			MaxSpeed = 0;

			if (CurrentSpeed == 0)
			{
				if (bIsMoveForward)
				{
					bIsMoveForward = false;
				}
				else
				{
					bIsMoveForward = true;
				}
				bIsReverseDirection = false;
				SetMaxSpeedToSpeedType();
			}
		}

		if (bIsMoveForward)
		{
			AddInputVector(PawnOwner->GetActorForwardVector());
		}
		else
		{
			AddInputVector(PawnOwner->GetActorForwardVector() * -1.0f);
		}
	}

	if (bCanRotation)
	{
		ChangeRotation(DeltaTime);
	}
}

void UWHCBattleShipMovement::InitBattleShipMovementComponent(float InitMaxMoveSpeed, float InitAcceleration, float InitDeceleration, float InitMaxRotSpeed, float InitRotAcceleration, float InitRotationAccelerationIncrease)
{
	InitMaxSpeed = InitMaxMoveSpeed;
	Acceleration = InitAcceleration;
	Deceleration = InitDeceleration;

	MaxRotationSpeed = InitMaxRotSpeed;
	InitMaxRotationSpeed = InitMaxRotSpeed;

	RotationAcceleration = InitRotAcceleration;
	InitRotationAcceleration = InitRotAcceleration;
	RotationAccelerationIncrease = InitRotationAccelerationIncrease;
}

void UWHCBattleShipMovement::SetMaxSpeedToSpeedType()
{
	if (InitMaxSpeed > 0)
	{
		if (SpeedType == ESpeedType::Back)
		{
			MaxSpeed = InitMaxSpeed / 4;
		}
		else if (SpeedType == ESpeedType::Stop)
		{
			MaxSpeed = 0;
		}
		else if (SpeedType == ESpeedType::OneQuater)
		{
			MaxSpeed = InitMaxSpeed / 4;
		}
		else if (SpeedType == ESpeedType::OneHalf)
		{
			MaxSpeed = InitMaxSpeed / 2;
		}
		else if (SpeedType == ESpeedType::ThreeQuater)
		{
			MaxSpeed = InitMaxSpeed / 4 * 3;
		}
		else if (SpeedType == ESpeedType::Max)
		{
			MaxSpeed = InitMaxSpeed;
		}
	}
}

void UWHCBattleShipMovement::SetSpeedType(uint8 TypeNum)
{
	ESpeedType ChangeType = static_cast<ESpeedType>(TypeNum);

	if (bCanChangeType)
	{
		SpeedType = ChangeType;

		SetMaxSpeedToSpeedType();
	}
}

void UWHCBattleShipMovement::IncreaseSpeedType()
{
	if (bCanChangeType)
	{
		if (SpeedType == ESpeedType::Back)
		{
			SpeedType = ESpeedType::Stop;
		}
		else if (SpeedType == ESpeedType::Stop)
		{
			SpeedType = ESpeedType::OneQuater;

		}
		else if (SpeedType == ESpeedType::OneQuater)
		{
			SpeedType = ESpeedType::OneHalf;
		}
		else if (SpeedType == ESpeedType::OneHalf)
		{
			SpeedType = ESpeedType::ThreeQuater;
		}
		else if (SpeedType == ESpeedType::ThreeQuater)
		{
			SpeedType = ESpeedType::Max;
		}

		SetMaxSpeedToSpeedType();

		if (!bIsMoveForward)
		{
			bIsReverseDirection = true;
		}
		else
		{
			if (bIsReverseDirection)
			{
				bIsReverseDirection = false;
			}
		}
	}


	// �ְ� �ӵ��� �� ������ �����Ϸ��� if �� �߰�
}

void UWHCBattleShipMovement::DecreaseSpeedType()
{
	if (bCanChangeType)
	{
		if (SpeedType == ESpeedType::Max)
		{
			SpeedType = ESpeedType::ThreeQuater;
		}
		else if (SpeedType == ESpeedType::ThreeQuater)
		{
			SpeedType = ESpeedType::OneHalf;
		}
		else if (SpeedType == ESpeedType::OneHalf)
		{
			SpeedType = ESpeedType::OneQuater;
		}
		else if (SpeedType == ESpeedType::OneQuater)
		{
			SpeedType = ESpeedType::Stop;
		}
		else if (SpeedType == ESpeedType::Stop)
		{
			SpeedType = ESpeedType::Back;
		}
		else if (SpeedType == ESpeedType::Back)
		{
			// ������ �� ������ �ȵȴٴ� �Ҹ��� ������ �߰��Ϸ��� ���� �ۼ� �߰�
		}

		SetMaxSpeedToSpeedType();

		if (bIsMoveForward)
		{
			bIsReverseDirection = true;
		}
		else
		{
			if (bIsReverseDirection)
			{
				bIsReverseDirection = false;
			}
		}
	}
}

void UWHCBattleShipMovement::ChangeRotation(float DeltaTime)
{
	if (bCanRotation)	// ȸ�� �, ����� �ƿ� ���� �������� false
	{
		float AbsTurnAngle = FMath::Abs(TurnAngle);

		// ȸ�� ���ӵ��� �ϸ��ϰ� ����
		if (InitRotationAcceleration > RotationAcceleration)
		{
			RotationAcceleration += InitRotationAcceleration * RotationAccelerationIncrease * DeltaTime;
		}

		// �ݴ� �������� ȸ���� �� õõ�� ȸ���ϱ� ���� bReverseDirection Ȯ��
		if (bReverseDirection)
		{
			if (RotationSpeed > 0)
			{
				RotationSpeed -= InitRotationAcceleration * DeltaTime * NormalizedSpeed;

				// ���������� ������ ��ȯ�� ��� RotationSpeed�� 0���� ���ҵ� ���� bReverseDirection ����
				if (RotationSpeed <= 0)
				{
					RotationSpeed = 0;
					bReverseDirection = false;
				}
			}
			else
			{
				// ó�� ������ȯ�� ������ ��� ����
				// ȸ�� ���ӵ� 0���� �ʱ�ȭ
				bReverseDirection = false;
				RotationAcceleration = 0;
			}
		}
		else if (bIsRotationDeceleration)
		{
			if (AbsTurnAngle > RotationSpeed)
			{
				bIsRotationDeceleration = false;
			}
			else
			{
				RotationSpeed = AbsTurnAngle;
			}

			if (RotationSpeed <= 0)
			{
				RotationSpeed = 0;
				bIsRotationDeceleration = false;
			}
		}
		// ������ �̵��� �ƴϰ� �������� �ʾƾ� �� ��� �������� ȸ���ӵ� ����
		else if (TurnAngle != 0)
		{
			if (MaxRotationSpeed > RotationSpeed)
			{
				RotationSpeed += RotationAcceleration * DeltaTime * NormalizedSpeed;

				if (MaxRotationSpeed < RotationSpeed)
				{
					RotationSpeed = MaxRotationSpeed;
				}
			}
			else if (MaxRotationSpeed < RotationSpeed)
			{
				RotationSpeed = MaxRotationSpeed;
			}
		}

		// TurnAngle�� �������� �󸶳� ȸ���ؾ��ϴ��� ���
		if (!bReverseDirection)
		{
			if (TurnAngle > 0)
			{
				TurnAngle -= RotationSpeed * DeltaTime;
				if (TurnAngle <= 0)
				{
					TurnAngle = 0;
				}
			}
			else if (TurnAngle < 0)
			{
				TurnAngle += RotationSpeed * DeltaTime;
				if (TurnAngle >= 0)
				{
					TurnAngle = 0;
				}
			}
		}

		if (AbsTurnAngle < RotationSpeed)
		{
			bIsRotationDeceleration = true;
		}
		if (bIsRotationDeceleration)
		{
			if (AbsTurnAngle > RotationSpeed)
			{
				bIsRotationDeceleration = false;
			}
		}

		// TurnAngle�� ���� Actor�� ������ ���� ȸ��
		if (TurnAngle != 0)
		{
			// ���� �����Ӱ� ���� ������ ȸ���ϴ� ��� ����
			if (!bReverseDirection)
			{
				if (TurnAngle > 0)
				{
					PawnOwner->AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
				}
				else
				{
					PawnOwner->AddActorLocalRotation(FRotator(0.0f, RotationSpeed * -1.0f * DeltaTime, 0.0f));
				}
			}
			// ���� �����Ӱ� �ݴ� �������� �����ؾ��ϴ� ��� ����
			else
			{
				if (TurnAngle < 0)
				{
					PawnOwner->AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
				}
				else
				{
					PawnOwner->AddActorLocalRotation(FRotator(0.0f, RotationSpeed * -1.0f * DeltaTime, 0.0f));
				}
			}

		}
		else
		{
			RotationSpeed = 0;
		}
	}
}

void UWHCBattleShipMovement::CalculateAngle(FVector HitPoint)
{
	FVector Point = HitPoint;
	FVector Start = PawnOwner->GetTransform().GetLocation();

	Point.Z = 0;
	Start.Z = 0;

	FVector Dir = Point - Start;

	float Dot = FVector::DotProduct(PawnOwner->GetActorForwardVector(), Dir.GetSafeNormal());
	float AcosAngle = FMath::Acos(Dot);
	float Angle = FMath::RadiansToDegrees(AcosAngle);

	FVector Cross = FVector::CrossProduct(PawnOwner->GetActorForwardVector(), Dir.GetSafeNormal());

	if (bIsMoveForward)
	{
		if (Cross.Z > 0)
		{
			if (bIsTurnLeft)
			{
				bReverseDirection = true;
				// ���������� �̵��� �� ȸ�� ���ӵ� �ʱ�ȭ
				RotationAcceleration = 0;
			}
			TurnAngle = Angle;
			bIsTurnLeft = false;
		}
		else if (Cross.Z < 0)
		{
			if (!bIsTurnLeft)
			{
				bReverseDirection = true;
				RotationAcceleration = 0;
			}
			TurnAngle = -Angle;
			bIsTurnLeft = true;
		}
	}
	else
	{
		if (Cross.Z > 0)
		{
			if (!bIsTurnLeft)
			{
				bReverseDirection = true;
				RotationAcceleration = 0;
			}
			TurnAngle = (Angle - 180);
			bIsTurnLeft = true;
		}
		else if (Cross.Z < 0)
		{
			if (bIsTurnLeft)
			{
				bReverseDirection = true;
				RotationAcceleration = 0;
			}
			TurnAngle = (180-Angle);
			bIsTurnLeft = false;
		}
	}
}
