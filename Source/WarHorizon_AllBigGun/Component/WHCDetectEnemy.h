#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WHCDetectEnemy.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARHORIZON_ALLBIGGUN_API UWHCDetectEnemy : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWHCDetectEnemy();

protected:
	virtual void BeginPlay() override;

	void DetectBattleShip();
	void DetectAircraft();
	void SortingArrayToDistance(TArray<APawn*> ArrayPawn);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitDetectEnemyComponent(uint8 TeamInt);
	void SetDetectedBattleShips(TArray<APawn*>* ArrayPtr) { DetectedBattleShips = ArrayPtr; }
	void SetDetectedAircrafts(TArray<APawn*>* ArrayPtr) { DetectedAircrafts = ArrayPtr; }

protected:
	TObjectPtr<APawn> PawnOwner;
	FTimerHandle BattleShipTimerHandle;
	FTimerHandle AircraftTimerHandle;

	FName BPresetName;
	FName APresetName;
	float BRadius = 6000.0f;
	float ARadius = 6000.0f;

	TArray<APawn*>* DetectedBattleShips;
	TArray<APawn*>* DetectedAircrafts;
};
