// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WHCDetectEnemy.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARHORIZON_ALLBIGGUN_API UWHCDetectEnemy : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWHCDetectEnemy();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitDetectEnemyComponent(uint8 TeamInt);
	void DetectBattleShip();
	void DetectAircraft();
	void SortingArrayToDistance(TArray<APawn*> ArrayPawn);


	void SetDetectedBattleShips(TArray<APawn*>* ArrayPtr) { DetectedBattleShips = *ArrayPtr; }
	void SetDetectedAircrafts(TArray<APawn*>* ArrayPtr) { DetectedAircrafts = *ArrayPtr; }
	TArray<APawn*> GetDetectedBattleShips() { return DetectedBattleShips; }
	TArray<APawn*> GetDetectedAircrafts() { return DetectedAircrafts; }

protected:
	TObjectPtr<APawn> PawnOwner;
	FTimerHandle BattleShipTimerHandle;
	FTimerHandle AircraftTimerHandle;

public:
	FName BPresetName;
	FName APresetName;
	float BRadius = 40000.0f;
	float ARadius = 40000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCDetectEnemy)
	TArray<APawn*> DetectedBattleShips;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCDetectEnemy)
	TArray<APawn*> DetectedAircrafts;
};
