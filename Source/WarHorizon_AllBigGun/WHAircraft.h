// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	//class StaticMeshComponent* StaticMeshComp;

	UStaticMesh* StaticMesh;
	UStaticMeshComponent* StaticMeshComp;

	APawn* Parent;
	UPROPERTY(VisibleAnywhere)
	FVector CurrentPosition;

	void MoveToCurrentPositionAndRotation(float DeltaTime);
};
