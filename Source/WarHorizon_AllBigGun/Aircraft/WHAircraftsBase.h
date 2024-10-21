// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/AircraftsInterface.h"
#include "Enum/EAircraftsType.h"
#include "WHAircraftsBase.generated.h"

class UWHCAircraftsMovement;
class AWHAircraft;

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHAircraftsBase : public APawn, public IAircraftsInterface
{
	GENERATED_BODY()

public:
	AWHAircraftsBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMesh* StaticMeshRef;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWHCAircraftsMovement> AircraftMovementComp;

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts")
	APawn* MotherShipPawn;

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts")
	uint32 ID;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts")
	FString Name;

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts")
	EAircraftsType AircraftsType;

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	int MaxHP;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	int CurrentHP;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	float MaxSpeed;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	float CurrentSpeed;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	float TurnAngle;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	float MaxHeight;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	float CurretHeight;

	UPROPERTY(VisibleAnywhere)
	bool IsSpawnEnd = false;

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	TArray<AWHAircraft*> ArrayAircrafts;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	uint32 AircraftsNum;

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	TArray<FVector> MovePoint;

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Attack")
	APawn* TargetAircrafts;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Attack")
	APawn* TargetBattleShip;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> InitAircraftPosition;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> AircraftFormations;

public:
	void InitToDataTable(int Id);
	void MoveFront();
	virtual void Turn(int Angle) override;
	virtual void IncreaseHeight(float MaxZ) override;
	virtual void DecreaseHeight(float MinZ) override;
	void GunAttack();
	void BombAttack();
	void ChangeMovePoint();
	virtual void SpawnAircraft(FVector StartPos) override;
	//virtual TArray<AActor*> GetArrayAircrafts() override;
	virtual uint8 GetAircraftType() override;
	virtual void DestroyAircraft(int Index) override;
	virtual APawn* GetMotherShip() override;
	virtual AActor* GetTargetAircrafts() override;
	void SetInitAircraftPosition();
	void SetAircraftFormations();
	void SetTargetAircraft();
	void SetTargetBattleShip();

	virtual void Fire() override;
};
