// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WHCAircraftsMovement.h"

UWHCAircraftsMovement::UWHCAircraftsMovement()
{
	PawnOwner = Cast<APawn>(GetOwner());
}

void UWHCAircraftsMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Turn(DeltaTime);
	//IncreaseHeight(DeltaTime);
}

void UWHCAircraftsMovement::IncreaseHeight(float DeltaTime)
{
	FRotator Rot;
	Rot = PawnOwner->GetActorRotation();
	if (Rot.Pitch <= 60.0f)
	{
		Rot.Pitch += 50.0f * DeltaTime;
		if (Rot.Pitch > 60.0f)
		{
			Rot.Pitch = 60.0f;
		}
		PawnOwner->SetActorRotation(Rot);
	}
}

void UWHCAircraftsMovement::DecreaseHeight(float DeltaTime)
{
	FRotator Rot;
	Rot = PawnOwner->GetActorRotation();
	if (Rot.Pitch >= -60.0f)
	{
		Rot.Pitch -= 50.0f * DeltaTime;
		if (Rot.Pitch < -60.0f)
		{
			Rot.Pitch = -60.0f;
		}
		PawnOwner->SetActorRotation(Rot);
	}
}

void UWHCAircraftsMovement::Turn(float DeltaTime)
{
	FRotator Rot;
	Rot = PawnOwner->GetActorRotation();
	Rot.Yaw += 20.0f * DeltaTime;
	PawnOwner->SetActorRotation(Rot);
}
