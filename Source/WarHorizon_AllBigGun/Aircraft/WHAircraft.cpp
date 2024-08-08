// Fill out your copyright notice in the Description page of Project Settings.


#include "Aircraft/WHAircraft.h"

// Sets default values
AWHAircraft::AWHAircraft()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
}

// Called when the game starts or when spawned
void AWHAircraft::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	MoveSpeed = MaxSpeed;

}

// Called every frame
void AWHAircraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToCurrentPositionAndRotation(DeltaTime);
}

void AWHAircraft::MoveToCurrentPositionAndRotation(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	
	FVector Dist = CurrentPosition - GetActorLocation();
	FVector Dir = Dist.GetSafeNormal();

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
	else if(DistanceFloat >150)
	{
		MoveSpeed = MaxSpeed * 1.2f;
	}
	else if(DistanceFloat > 100)
	{
		MoveSpeed = MaxSpeed * 1.1f;
	}
	else if(DistanceFloat > 50)
	{
		MoveSpeed = MaxSpeed * 0.9f;
	}
	else
	{
		MoveSpeed = MaxSpeed * 0.8f;
	}
	
	CurrentLocation += Dir * MoveSpeed * DeltaTime;

	SetActorLocation(CurrentLocation);
	SetActorRotation(Dir.ToOrientationRotator());
}

