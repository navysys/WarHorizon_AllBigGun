// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WHCAircraftsMovement.h"

UWHCAircraftsMovement::UWHCAircraftsMovement()
{
	PawnOwner = Cast<APawn>(GetOwner());

	PitchRotSpeed = 30.0f;
	// 공격 모드 일때는 60~70 정도로
	YawRotSpeed = 40.0f;
}

void UWHCAircraftsMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickDeltaTime = DeltaTime;
	
	//Turn();
	//IncreaseHeight();
}

void UWHCAircraftsMovement::IncreaseHeight(float MaxZ)
{
	FVector Loc = GetActorLocation();
	FRotator Rot = PawnOwner->GetActorRotation();
	if (MaxZ > Loc.Z)
	{
		if (Rot.Pitch <= PitchRotSpeed)
		{
			Rot.Pitch += PitchRotSpeed * 0.8f * TickDeltaTime;
			if (Rot.Pitch > PitchRotSpeed)
			{
				Rot.Pitch = PitchRotSpeed;
			}
			PawnOwner->SetActorRotation(Rot);
		}
	}
	else if (MaxZ < Loc.Z)
	{
		if (Rot.Pitch > 0)
		{
			Rot.Pitch -= PitchRotSpeed * TickDeltaTime;
			if (Rot.Pitch < 0)
			{
				Rot.Pitch = 0;
			}
		}
		PawnOwner->SetActorRotation(Rot);
		PawnOwner->SetActorLocation(FVector(Loc.X, Loc.Y, MaxZ));
	}

}

void UWHCAircraftsMovement::DecreaseHeight(float MinZ)
{
	FVector Loc = GetActorLocation();
	FRotator Rot = PawnOwner->GetActorRotation();
	if (MinZ < Loc.Z)
	{
		if (Rot.Pitch >= -PitchRotSpeed)
		{
			Rot.Pitch -= PitchRotSpeed * 0.8f * TickDeltaTime;
			if (Rot.Pitch < -PitchRotSpeed)
			{
				Rot.Pitch = -PitchRotSpeed;
			}
			PawnOwner->SetActorRotation(Rot);
		}
	}
	else if (MinZ > Loc.Z)
	{
		if (Rot.Pitch < 0)
		{
			Rot.Pitch += PitchRotSpeed * TickDeltaTime;
			if (Rot.Pitch > 0)
			{
				Rot.Pitch = 0;
			}
		}
		PawnOwner->SetActorRotation(Rot);
		PawnOwner->SetActorLocation(FVector(Loc.X, Loc.Y, MinZ));
	}
}

void UWHCAircraftsMovement::Turn(int Angle)
{
	if (Angle != 0)
	{
		FRotator Rot = PawnOwner->GetActorRotation();
		if (abs(Angle) > YawRotSpeed)
		{
			if (Angle > 1)
			{
				Rot.Yaw -= YawRotSpeed * TickDeltaTime;
			}
			else if (Angle < -1)
			{
				Rot.Yaw += YawRotSpeed * TickDeltaTime;
			}
		}
		else
		{
			Rot.Yaw -= Angle * TickDeltaTime;
		}
		PawnOwner->SetActorRotation(Rot);
	}

}
