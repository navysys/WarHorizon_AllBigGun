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


	// 최고 속도일 때 눌러도 반응하려면 if 문 추가
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
			// 후진일 때 눌러도 안된다는 소리나 반응을 추가하려면 내용 작성 추가
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
	if (bCanRotation)	// 회전 운동, 계산을 아예 하지 않으려면 false
	{
		float AbsTurnAngle = FMath::Abs(TurnAngle);

		// 회전 가속도를 완만하게 증가
		if (InitRotationAcceleration > RotationAcceleration)
		{
			RotationAcceleration += InitRotationAcceleration * RotationAccelerationIncrease * DeltaTime;
		}

		// 반대 방향으로 회전할 때 천천히 회전하기 위한 bReverseDirection 확인
		if (bReverseDirection)
		{
			if (RotationSpeed > 0)
			{
				RotationSpeed -= InitRotationAcceleration * DeltaTime * NormalizedSpeed;

				// 역방향으로 방향을 전환할 경우 RotationSpeed가 0으로 감소된 이후 bReverseDirection 변경
				if (RotationSpeed <= 0)
				{
					RotationSpeed = 0;
					bReverseDirection = false;
				}
			}
			else
			{
				// 처음 방향전환을 실행할 경우 실행
				// 회전 가속도 0으로 초기화
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
		// 역방향 이동이 아니고 감속하지 않아야 할 경우 정상적인 회전속도 증가
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

		// TurnAngle을 기준으로 얼마나 회전해야하는지 계산
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

		// TurnAngle을 토대로 Actor의 방향을 직접 회전
		if (TurnAngle != 0)
		{
			// 이전 프레임과 같은 정방향 회전하는 경우 실행
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
			// 이전 프레임과 반대 방향으로 진행해야하는 경우 실행
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
				// 역방향으로 이동할 때 회전 가속도 초기화
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
