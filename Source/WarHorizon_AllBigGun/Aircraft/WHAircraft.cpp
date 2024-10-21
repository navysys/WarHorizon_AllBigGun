#define LANDINGSPEEDLIMIT 0.5f
#define FOLLOWGROUPHEIGHTDECEL 0.75f

#include "Aircraft/WHAircraft.h"
#include "Kismet/KismetMathLibrary.h"


AWHAircraft::AWHAircraft()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RootComponent = StaticMeshComp;
}

void AWHAircraft::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	MaxSpeed = InitMaxSpeed;
	MoveSpeed = 0.0f;
	LandingTime = 1.5f;
	TargetHeight = 12000.0f;
	HeightAcceleration = 2000.0f;

	GetWorld()->GetTimerManager().SetTimer(AircraftTimer, this, &AWHAircraft::CalculateDist, 0.1f, true);

	//변수 init 함수 만들어서 수치 aircrafts base 에서 받아서 동작하도록 이후 수정
}

void AWHAircraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (AircraftState)
	{
		case EAircraftState::Landing:
		{
			// 테스트용으로 MaxSpeed 직접 변경
			MaxSpeed = InitMaxSpeed;
			MoveSpeed += MaxSpeed * (LANDINGSPEEDLIMIT / LandingTime) * DeltaTime;
			if (MaxSpeed * LANDINGSPEEDLIMIT < MoveSpeed)
			{
				MoveSpeed = MaxSpeed * LANDINGSPEEDLIMIT;
				AircraftState = EAircraftState::FollowGroup;
			}

			FVector Loc = GetActorLocation();
			FVector NextPos = Loc + MoveSpeed * DeltaTime;

			SetActorLocation(NextPos);
			break;
		}

		case EAircraftState::FollowGroup:
		{
			FVector Loc = GetActorLocation();
			// 움직임 관련 (XY 계산)
			FVector ForwardXY = GetActorForwardVector().GetSafeNormal2D();


			// 현재 속도 제어
			if (MaxSpeed > MoveSpeed)
			{
				MoveSpeed += MaxSpeed * DeltaTime;
				if (MaxSpeed < MoveSpeed)
				{
					MoveSpeed = MaxSpeed;
				}
			}
			
			// 움직임 관련 (Z 계산)
			CurrentHeight = GetActorLocation().Z;
			if (TargetHeight > CurrentHeight)
			{
				CurrentHeight += HeightAcceleration * FOLLOWGROUPHEIGHTDECEL * DeltaTime;
				if (CurrentHeight > TargetHeight)
				{
					CurrentHeight = TargetHeight;
				}
			}
			else if (TargetHeight < CurrentHeight)
			{
				CurrentHeight -= HeightAcceleration * FOLLOWGROUPHEIGHTDECEL * DeltaTime;
				if (CurrentHeight < TargetHeight)
				{
					CurrentHeight = TargetHeight;
				}
			}
			Loc.Z = CurrentHeight;

			FVector NextPos = Loc + ForwardXY * MoveSpeed * DeltaTime;
			SetActorLocation(NextPos);

			// 회전 관련 (Yaw)
			RotationTime += DeltaTime;
			if (RotationTime >= 0.05f)
			{
				FRotator CurRot = GetActorRotation();
				if (Angle > 0)
				{
					CurRot.Yaw += 1;
				}
				else if (Angle < 0)
				{
					CurRot.Yaw -= 1;
				}
				SetActorRotation(CurRot);
				RotationTime -= 0.05f;
			}

			
			
			
			break;
		}

		case EAircraftState::Normal:
		{
			MaxSpeed = InitMaxSpeed;
			FVector Loc = GetActorLocation();
			FVector ForwardXY = GetActorForwardVector().GetSafeNormal2D();
			FVector RightVec = GetActorRightVector();
			RightVec.Z = 0;
			
			FVector Dir = (CurrentPosition - Loc).GetSafeNormal2D();
			float DotProduct = FVector::DotProduct(RightVec, Dir);
			
			FVector CorrectionY;

			if (DotProduct < 0) // 왼쪽
			{
				CorrectionY = RightVec * -1.0f;
			}
			else if (DotProduct > 0)
			{
				CorrectionY = RightVec;
			}

			if (MaxSpeed > MoveSpeed)
			{
				MoveSpeed += MaxSpeed * DeltaTime;
				if (MaxSpeed < MoveSpeed)
				{
					MoveSpeed = MaxSpeed;
				}
			}
			else if (MaxSpeed < MoveSpeed)
			{
				MoveSpeed -= MaxSpeed * DeltaTime;
				if (MaxSpeed > MoveSpeed)
				{
					MoveSpeed = MaxSpeed;
				}
			}
			
			// 움직임 관련 (Z 계산)
			CurrentHeight = Loc.Z;
			if (TargetHeight > CurrentHeight)
			{
				CurrentHeight += HeightAcceleration * DeltaTime;
				if (CurrentHeight > TargetHeight)
				{
					CurrentHeight = TargetHeight;
				}
			}
			else if (TargetHeight < CurrentHeight)
			{
				CurrentHeight -= HeightAcceleration * DeltaTime;
				if (CurrentHeight < TargetHeight)
				{
					CurrentHeight = TargetHeight;
				}
			}
			Loc.Z = CurrentHeight;

			FVector NextPos = Loc + ForwardXY * MoveSpeed * DeltaTime; // CorrectionY 추가해야함
			SetActorLocation(NextPos);

			// 회전 관련 (Yaw)
			RotationTime += DeltaTime;
			if (RotationTime >= 0.05f)
			{
				FRotator CurRot = GetActorRotation();
				if (Angle > 0)
				{
					CurRot.Yaw += 1;
				}
				else if (Angle < 0)
				{
					CurRot.Yaw-= 1;
				}
				SetActorRotation(CurRot);
				RotationTime -= 0.05f;
			}

			break;
		}

		case EAircraftState::AtA:
		{
			// 타겟 추적
			// 종료 후 FollwGroup
			break;
		}

		case EAircraftState::AtB:
		{
			// 공격 준비 상태 진입
			// 진입 상태에서 일정 조건 달성 or 공격 한번 더 입력으로 공격
			// 종료 후 FollwGroup
			break;
		}

		case EAircraftState::Invalid:
		{
			break;
		}
		
		default:
		{
			
			//원하지 않은 상태
			break;
		}
	}


}

void AWHAircraft::CalculateDist()
{

	FVector Loc = GetActorLocation();
	Loc.Z = 0;
	FVector CurrentPos2D = CurrentPosition;
	CurrentPos2D.Z = 0;

	// 최대 속도 제어
	Dist = FVector::Dist(Loc, CurrentPos2D);

	switch (AircraftState)
	{
	case EAircraftState::Landing:
		break;

	case EAircraftState::FollowGroup:
	{
		FVector ForwardXY = GetActorForwardVector().GetSafeNormal2D();
		FVector DirXY = (CurrentPos2D - Loc).GetSafeNormal2D();
		float Dot = FVector::DotProduct(ForwardXY, DirXY);

		// 속도 관련
		if (Dist < 100.0f || Dot < 0)
		{
			MaxSpeed = InitMaxSpeed;
			AircraftState = EAircraftState::Normal;
		}
		else if (Dist < 250.0f)
		{
			MaxSpeed = InitMaxSpeed * 1.2f;
		}
		else if (Dist < 500.0f)
		{
			MaxSpeed = InitMaxSpeed * 1.4f;
		}
		else if (Dist < 1000.0f)
		{
			MaxSpeed = InitMaxSpeed * 1.6f;
		}
		else
		{
			MaxSpeed = InitMaxSpeed * 2.0f;
		}

		if (Dot > 0)
		{
			// 회전 관련 (Yaw)
			FRotator Rot = GetActorRotation();
			float CurrentYaw = Rot.Yaw;

			FVector TargetDir = (CurrentPosition - Loc).GetSafeNormal2D();
			float TargetYaw = TargetDir.Rotation().Yaw;

			Angle = TargetYaw - CurrentYaw;
			if (Angle > 180)
			{
				Angle -= 360;
			}
			else if (Angle < -180)
			{
				Angle += 360;
			}
		}
		else if (Dot < 0)
		{
			Angle = HeadDir.Yaw - GetActorRotation().Yaw;
		}


		break;
	}

	case EAircraftState::Normal:
	{
		FVector ForwardXY = GetActorForwardVector().GetSafeNormal2D();
		FVector DirXY = (Loc - CurrentPosition).GetSafeNormal2D();
		float Dot = FVector::DotProduct(ForwardXY, DirXY);
		if (Dot > 0)
		{
			if (Dist < 20.0f)
			{
				MaxSpeed = InitMaxSpeed;
			}
			else if (Dist < 100.0f)
			{
				MaxSpeed = InitMaxSpeed * 1.2f;
			}
			else
			{
				MaxSpeed = InitMaxSpeed * 1.4f;
			}
		}
		else if (Dot < 0)
		{
			if (Dist < 20.0f)
			{
				MaxSpeed = InitMaxSpeed;
			}
			else if (Dist < 100.0f)
			{
				MaxSpeed = InitMaxSpeed * 0.8f;
			}
			else
			{
				MaxSpeed = InitMaxSpeed * 0.6f;
			}
		}

		FRotator Rot = GetActorRotation();
		float CurrentYaw = Rot.Yaw;
		float AircraftsYaw = HeadDir.Yaw;

		Angle = AircraftsYaw - CurrentYaw;
		if (Angle > 180)
		{
			Angle -= 360;
		}
		else if (Angle < -180)
		{
			Angle += 360;
		}

		break;
	}

	case EAircraftState::AtA:
		break;
	case EAircraftState::AtB:
		break;
	case EAircraftState::Invalid:
		break;
	default:
		break;
	}
}

