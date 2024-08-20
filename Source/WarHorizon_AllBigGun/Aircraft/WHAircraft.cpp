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
}

// Called every frame
void AWHAircraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToCurrentPositionAndRotation(DeltaTime);
	// Aircrafts 클래스에서 방향과 거리만 전달 받고 지정된 위치를 지키지 않도록 수정해야함
	switch (AircraftState)
	{
	case EAircraftState::Landing:
		// 처음 스폰 이후 활주로 직선으로 이동
		break;
	case EAircraftState::FollowGroup:
		//랜딩 이후 빠른 속도로 그룹을 향해 이동
		break;
	case EAircraftState::Normal:
		break;
	case EAircraftState::AtA:
		// 타겟 추적
		break;
	case EAircraftState::AtB:
		// 공격 준비 상태 진입
		// 진입 상태에서 일정 조건 달성 or 공격 한번 더 입력으로 공격
		break;
	case EAircraftState::Invalid:
		break;
	default:
		//원하지 않은 상태 오류 띄워야함
		break;
	}
}

void AWHAircraft::MoveToCurrentPositionAndRotation(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	
	FVector Dist = CurrentPosition - GetActorLocation();
	FVector Dir = Dist.GetSafeNormal();
	float Alpha = 1;
	if (IsSpawnEnd)
	{
		Alpha = 0;
	}
	else
	{
		Alpha = 0.9f;
	}

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

