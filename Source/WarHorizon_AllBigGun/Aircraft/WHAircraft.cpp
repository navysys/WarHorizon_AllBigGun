#define LANDINGSPEEDLIMIT 0.5f
#define FOLLOWGROUPHEIGHTDECEL 0.75f

// Fill out your copyright notice in the Description page of Project Settings.
#include "Aircraft/WHAircraft.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AWHAircraft::AWHAircraft()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RootComponent = StaticMeshComp;
}

// Called when the game starts or when spawned
void AWHAircraft::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	MaxSpeed = InitMaxSpeed;
	MoveSpeed = 0.0f;
	LandingTime = 1.5f;
	TargetHeight = 12000.0f;
	HeightAcceleration = 2000.0f;

	//���� init �Լ� ���� ��ġ aircrafts base ���� �޾Ƽ� �����ϵ��� ���� ����
}

// Called every frame
void AWHAircraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (AircraftState)
	{
		case EAircraftState::Landing:
		{
			// �׽�Ʈ������ MaxSpeed ���� ����
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
			// ������ ���� (�ӵ�)

			FVector Loc = GetActorLocation();
			FVector Loc2D = Loc;
			Loc2D.Z = 0;
			FVector CurrentPos2D = CurrentPosition;
			CurrentPos2D.Z = 0;

			// �ִ� �ӵ� ����
			float Dist = FVector::Dist(Loc2D, CurrentPos2D);
			if (Dist < 10.0f)
			{
				MaxSpeed = InitMaxSpeed;
				AircraftState = EAircraftState::Normal;
			}
			else if (Dist < 250.0f)
			{
				MaxSpeed = InitMaxSpeed * 1.4f;
			}
			else if (Dist < 500.0f)
			{
				MaxSpeed = InitMaxSpeed * 1.6f;
			}
			else if (Dist < 1000.0f)
			{
				MaxSpeed = InitMaxSpeed * 1.8f;
			}
			else
			{
				MaxSpeed = InitMaxSpeed * 2.0f;
			}

			// ���� �ӵ� ����
			if (MaxSpeed > MoveSpeed)
			{
				MoveSpeed += MaxSpeed * DeltaTime;
				if (MaxSpeed < MoveSpeed)
				{
					MoveSpeed = MaxSpeed;
				}
			}
			
			// ������ ���� (XY ���)
			FVector ForwardXY = GetActorForwardVector().GetSafeNormal2D();

			// ������ ���� (Z ���)
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
			

			// ȸ�� ���� (Yaw)
			FRotator Rot = GetActorRotation();
			float CurrentYaw = Rot.Yaw;

			FVector TargetDir = (CurrentPosition - Loc).GetSafeNormal2D();
			float TargetYaw = TargetDir.Rotation().Yaw;

			float Angle = CurrentYaw - TargetYaw;
			if (Angle > 180)
			{
				Angle -= 360;
			}
			else if (Angle < -180)
			{
				Angle += 360;
			}

			if (FMath::IsNearlyZero(Angle, 0.1) == false)
			{
				if (Angle > 0)
				{
					CurrentYaw -= 45.0f * DeltaTime;
				}
				else if (Angle < 0)
				{
					CurrentYaw += 45.0f * DeltaTime;
				}
			}
			// ���� �� �ִ��� �˻��ϴ� �ڵ� ���� �ʿ� X
			// 
			//else
			//{
			//	CurrentYaw = TargetYaw;

			//	FVector NextPos2D = NextPos;
			//	NextPos2D.Z = 0;

			//	float CurrentDist = FVector::Dist(CurrentPos2D, NextPos2D);
			//	FVector LineEnd = NextPos2D + ForwardXY * CurrentDist;
			//	float LineDist = FVector::Dist(LineEnd, CurrentPos2D);

			//	UE_LOG(LogTemp, Warning, TEXT("LineEnd : %s"), *LineEnd.ToString());
			//	UE_LOG(LogTemp, Warning, TEXT("LineDist : %f"), LineDist);

			//	if (FMath::IsNearlyZero(LineDist, 0.001) == false)
			//	{
			//		int LineAngle = (CurrentPos2D - LineEnd).GetSafeNormal2D().Rotation().Yaw;
			//		UE_LOG(LogTemp, Warning, TEXT("Angle : %i"), LineAngle);
			//		if (LineAngle > 180)
			//		{
			//			LineAngle -= 360;
			//		}
			//		else if (LineAngle < -180)
			//		{
			//			LineAngle += 360;
			//		}

			//		if (LineAngle > 0)
			//		{
			//			//NextPos += GetActorRightVector().GetSafeNormal2D() * MoveSpeed * 0.2f * DeltaTime;
			//		}
			//		else if (LineAngle < 0)
			//		{
			//			//NextPos -= GetActorRightVector().GetSafeNormal2D() * MoveSpeed * 0.2f * DeltaTime;
			//		}
			//	}

				//int Angle = Dir.Rotation().Yaw - HeadDir.Yaw;



				//if (Angle > 0)
				//{
				//	UE_LOG(LogTemp, Warning, TEXT("Angle > : %i"), Angle);
				//	NextPos += GetActorRightVector().GetSafeNormal2D() * MoveSpeed * 0.2f * DeltaTime;
				//}
				//else if (Angle < 0)
				//{
				//	UE_LOG(LogTemp, Warning, TEXT("Angle < : %i"), Angle);
				//	NextPos -= GetActorRightVector().GetSafeNormal2D() * MoveSpeed * 0.2f * DeltaTime;
				//}
				//else
				//{
				//	UE_LOG(LogTemp, Warning, TEXT("Angle = 0 : %i"), Angle);
				//}
			//}

			Rot.Yaw = CurrentYaw;

			SetActorLocation(NextPos);
			SetActorRotation(Rot);
			
			break;
		}

		case EAircraftState::Normal:
		{
			MaxSpeed = InitMaxSpeed;
			FVector Loc = GetActorLocation();
			FVector ForwardXY = GetActorForwardVector().GetSafeNormal2D();
			FVector DirXY = (Loc - CurrentPosition).GetSafeNormal2D();
			FVector Loc2D = Loc;
			Loc2D.Z = 0;
			FVector CurrentPos2D = CurrentPosition;
			CurrentPos2D.Z = 0;
			
			float Dist = FVector::Dist(Loc2D, CurrentPos2D);
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
			
			//UE_LOG(LogTemp, Warning, TEXT("Dot : %f"), Dot);
			// ������ ���� (�ӵ�)
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
			
			// ������ ���� (Z ���)
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

			FVector NextPos = Loc + ForwardXY * MoveSpeed * DeltaTime;


			// ȸ�� ���� (Yaw)
			FRotator Rot = GetActorRotation();
			float CurrentYaw = Rot.Yaw;
			int IntCurrentYaw = CurrentYaw;
			int AircraftsYaw = HeadDir.Yaw;

			if (IntCurrentYaw > AircraftsYaw)
			{
				CurrentYaw -= 45.0f * DeltaTime;
			}
			else if (IntCurrentYaw < AircraftsYaw)
			{
				CurrentYaw += 45.0f * DeltaTime;
			}

			Rot.Yaw = CurrentYaw;

			SetActorLocation(NextPos);
			SetActorRotation(Rot);

			break;
		}

		case EAircraftState::AtA:
		{
			// Ÿ�� ����
			// ���� �� FollwGroup
			break;
		}

		case EAircraftState::AtB:
		{
			// ���� �غ� ���� ����
			// ���� ���¿��� ���� ���� �޼� or ���� �ѹ� �� �Է����� ����
			// ���� �� FollwGroup
			break;
		}

		case EAircraftState::Invalid:
		{
			break;
		}
		
		default:
		{
			
			//������ ���� ����
			break;
		}
	}


}

void AWHAircraft::MoveToCurrentPositionAndRotation(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	
	FVector Dist = CurrentPosition - GetActorLocation();
	FVector Dir = Dist.GetSafeNormal();
	float Alpha = 1;

	// ���� ����� �� ���� �Ÿ��� �پ��� �ӵ��� ���� �����ϰ� ����
	float DistanceFloat = FVector::Dist(FVector(0,0,0), Dist);
	if (DistanceFloat > 400)
	{
		MoveSpeed = MaxSpeed * 2.0f;
	}
	if (DistanceFloat > 250)
	{
		MoveSpeed = MaxSpeed * 1.8f;
	}
	else if(DistanceFloat >200)
	{
		MoveSpeed = MaxSpeed * 1.1f;
	}
	else if(DistanceFloat > 150)
	{
		MoveSpeed = MaxSpeed * 0.9f;
	}
	else if(DistanceFloat > 100)
	{
		MoveSpeed = MaxSpeed * 0.8f;
	}
	else if(DistanceFloat > 50)
	{
		MoveSpeed = MaxSpeed * 0.6f;
	}
	else
	{
		MoveSpeed = MaxSpeed * 0.1f;
	}
	
	CurrentLocation += Dir * MoveSpeed * DeltaTime;

	FRotator MidRotator = UKismetMathLibrary::RLerp(Dir.ToOrientationRotator(), HeadDir, Alpha, true);

	SetActorLocation(CurrentLocation);
	SetActorRotation(MidRotator);
}

