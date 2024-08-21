// Fill out your copyright notice in the Description page of Project Settings.
#include "Aircraft/WHAircraft.h"
#include "Kismet/KismetMathLibrary.h"

#define LANDINGSPEEDLIMIT 0.5f
#define FOLLOWGROUPBOOST 1.4f
#define FOLLOWGROUPHEIGHTDECEL 0.75f

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
	MoveSpeed = 0.0f;
	LandingTime = 1.0f;
	TargetHeight = 12000.0f;
	HeightAcceleration = 2000.0f;
}

// Called every frame
void AWHAircraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//MoveToCurrentPositionAndRotation(DeltaTime);
	// Aircrafts 클래스에서 방향과 거리만 전달 받고 지정된 위치를 지키지 않도록 수정해야함
	switch (AircraftState)
	{
		case EAircraftState::Landing:
		{
			// 처음 스폰 이후 활주로 직선으로 이동
			// 속도 0 ~ 최대 속도 절반까지 증가
			// 고도 상승 X 방향 전환 X
			// 기본 이동속도 도달하면 FloowGroup
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
			// 움직임 관련
			if (MaxSpeed * FOLLOWGROUPBOOST > MoveSpeed)
			{
				MoveSpeed += MaxSpeed * FOLLOWGROUPBOOST * DeltaTime;
				if (MaxSpeed * FOLLOWGROUPBOOST < MoveSpeed)
				{
					MoveSpeed = MaxSpeed * FOLLOWGROUPBOOST;
				}
			}

			FVector Loc = GetActorLocation();
			// XY 계산
			FVector ForwardXY = GetActorForwardVector().GetSafeNormal2D();

			// Z 계산
			CurrentHeight = Loc.Z;
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

			// 회전 관련
			FRotator Rot = GetActorRotation();
			float CurrentYaw = Rot.Yaw;
			float AircraftsYaw = HeadDir.Yaw;

			if (CurrentYaw > AircraftsYaw)
			{
				CurrentYaw -= 10.0f * DeltaTime;
			}
			else if (CurrentYaw < AircraftsYaw)
			{
				CurrentYaw += 10.0f * DeltaTime;
			}
			Rot.Yaw = CurrentYaw;

			SetActorRotation(Rot);
			
			break;
		}

		case EAircraftState::Normal:
		{
			FVector Loc = GetActorLocation();
			// XY 계산
			FVector ForwardXY = GetActorForwardVector().GetSafeNormal2D();

			// Z 계산
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
			SetActorLocation(NextPos);

			break;
		}

		case EAircraftState::AtA:
		{
			// 타겟 추적
			// 종료 후 FloowGroup
			break;
		}

		case EAircraftState::AtB:
		{
			// 공격 준비 상태 진입
			// 진입 상태에서 일정 조건 달성 or 공격 한번 더 입력으로 공격
			// 종료 후 FloowGroup
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

void AWHAircraft::MoveToCurrentPositionAndRotation(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	
	FVector Dist = CurrentPosition - GetActorLocation();
	FVector Dir = Dist.GetSafeNormal();
	float Alpha = 1;

	// 차후 상승할 때 직선 거리가 줄어들고 속도가 조금 감소하게 수정
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

