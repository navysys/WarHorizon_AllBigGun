// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FOW_Obstacle.generated.h"

class USphereComponent;
class AFOW_Manager;

UCLASS()
class WARHORIZON_ALLBIGGUN_API AFOW_Obstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFOW_Obstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void IncreaceSmoke(float DeltaTime);
	void DecreaseSmoke(float DeltaTime);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fog of War")
	UStaticMeshComponent* ObstacleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog of War")
	USphereComponent* CollisionComp;

	AFOW_Manager* FogManager;
	
	bool bIsIncreasingSmoke = false;
	float SmokeParameter = 0.0f;
	float LifeTime = 20.0f;
	float ElapsedTime = 0.0f;
	float SmokeDeploySpeed = 5.0f;
};
