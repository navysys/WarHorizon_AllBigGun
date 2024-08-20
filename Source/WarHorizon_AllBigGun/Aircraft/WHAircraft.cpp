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
	// Aircrafts Ŭ�������� ����� �Ÿ��� ���� �ް� ������ ��ġ�� ��Ű�� �ʵ��� �����ؾ���
	switch (AircraftState)
	{
	case EAircraftState::Landing:
		// ó�� ���� ���� Ȱ�ַ� �������� �̵�
		break;
	case EAircraftState::FollowGroup:
		//���� ���� ���� �ӵ��� �׷��� ���� �̵�
		break;
	case EAircraftState::Normal:
		break;
	case EAircraftState::AtA:
		// Ÿ�� ����
		break;
	case EAircraftState::AtB:
		// ���� �غ� ���� ����
		// ���� ���¿��� ���� ���� �޼� or ���� �ѹ� �� �Է����� ����
		break;
	case EAircraftState::Invalid:
		break;
	default:
		//������ ���� ���� ���� �������
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

