// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WHIslandBase.generated.h"

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHIslandBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWHIslandBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void Capture();
	void LevelUp();
protected:
	float HP;
	uint16 Level;
	bool bIsActivate;

	float CapturePoint;
	bool bIsCapturing;
public:

};
