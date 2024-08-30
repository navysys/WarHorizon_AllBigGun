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
	// Sets default values for this actor's properties
	AWHAircraft();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere)
	//USceneComponent* SceneComp;
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
	FVector CurrentPosition;

	void CalculateDist();
};
