// Fill out your copyright notice in the Description page of Project Settings.


#include "WHCAircraftsMovement.h"

void UWHCAircraftsMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector Location = GetActorLocation();
	//if (CurrentHeight != Location.Z)
	//{
	//	if (CurrentHeight > Location.Z)
	//	{
	//		IncreaseHeight(DeltaTime, Location);
	//	}
	//	else if(CurrentHeight < Location.Z)
	//	{
	//		DecreaseHeight(DeltaTime, Location);
	//	}
	//}
}

void UWHCAircraftsMovement::IncreaseHeight(float DeltaTime, FVector Loc)
{
	//상승 속도 점차 증가하도록 최대치 변수 따로
	//if (MaxAscentSpeed > AscentSpeed)
	//{
	//	AscentSpeed += AscentAcceleration * DeltaTime;
	//	if (Loc.Z + AscentSpeed > CurrentHeight)
	//	{
	//		AscentSpeed = CurrentHeight - Loc.Z;
	//	}
	//}

	FVector Location = Loc;
	Location.Z += AscentSpeed * DeltaTime;
	PawnOwner->SetActorLocation(Location);

	if (Location.Z >= 5900.0f)
	{
		CurrentHeight = 1000.0f;
	}
}

void UWHCAircraftsMovement::DecreaseHeight(float DeltaTime, FVector Loc)
{
	//if (MaxAscentSpeed > DescentSpeed)
	//{
	//	DescentSpeed += AscentAcceleration * DeltaTime;
	//	if (Loc.Z - DescentSpeed < CurrentHeight)
	//	{
	//		DescentSpeed = Loc.Z - CurrentHeight;
	//	}
	//}

	FVector Location = Loc;
	Location.Z -= DescentSpeed * DeltaTime;
	PawnOwner->SetActorLocation(Location);

	if (Location.Z <= 1100.0f)
	{
		CurrentHeight = 6000.0f;
	}
}
