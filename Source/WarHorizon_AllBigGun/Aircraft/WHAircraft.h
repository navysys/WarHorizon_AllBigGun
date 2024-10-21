// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/EAircraftState.h"
#include "WHAircraft.generated.h"

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHAircraft : public AActor
{
	GENERATED_BODY()
	
public:	
	AWHAircraft();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UStaticMesh* StaticMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	EAircraftState AircraftState = EAircraftState::Landing;

	FTimerHandle AircraftTimer;

	UPROPERTY(VisibleAnywhere)
	FRotator HeadDir;

	UPROPERTY(VisibleAnywhere)
	float InitMaxSpeed;

	UPROPERTY(VisibleAnywhere)
	float MaxSpeed;

	UPROPERTY(VisibleAnywhere)
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere)
	float TargetHeight;

	UPROPERTY(VisibleAnywhere)
	float CurrentHeight;

	UPROPERTY(VisibleAnywhere)
	float HeightAcceleration;

	UPROPERTY(VisibleAnywhere)
	float LandingTime;

	float Dist;
	UPROPERTY(VisibleAnywhere)
	float RotationTime;
	UPROPERTY(VisibleAnywhere)
	int Angle;


	UPROPERTY(VisibleAnywhere)
	FVector CurrentPosition;

	void CalculateDist();
};
