// Fill out your copyright notice in the Description page of Project Settings.


#include "WHAircraft.h"

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

	//UE_LOG(LogTemp, Warning, TEXT("Dist = %s, Dir = %s"), *Dist.ToString(), *Dir.ToString());

	float MoveSpeed = 1500.0f; // 차후 상승할 때 직선 거리가 줄어들고 속도가 조금 감소하게 수정
	
	CurrentLocation += Dir * MoveSpeed * DeltaTime;

	SetActorLocation(CurrentLocation);
	SetActorRotation(Dir.ToOrientationRotator());
}

