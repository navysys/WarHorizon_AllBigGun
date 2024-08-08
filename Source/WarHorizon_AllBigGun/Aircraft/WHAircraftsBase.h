// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/AircraftsInterface.h"
#include "Enum/EAircraftType.h"
#include "WHAircraftsBase.generated.h"

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHAircraftsBase : public APawn, public IAircraftsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWHAircraftsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(VisibleAnywhere)
	UStaticMesh* StaticMeshRef;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWHCAircraftsMovement> AircraftMovementComp;

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts")
	uint32 ID;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts")
	FString Name;

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts")
	EAircraftType AircraftType;

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

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	TArray<class AWHAircraft*> ArrayAircrafts;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	uint32 AircraftsNum;

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Stat")
	TArray<FVector> MovePoint;

	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Attack")
	TArray<APawn*> TargetAircrafts;
	UPROPERTY(VisibleAnywhere, Category = "Aircrafts | Attack")
	APawn* TargetBattleShip;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> InitAircraftPosition;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> AircraftFormations;

public:
	void MoveFront();
	void IncreaseHeight();
	void DecreaseHeight();
	void GunAttack();
	void BombAttack();
	void ChangeMovePoint();
	virtual void AddAircraft(FVector StartPos) override;
	virtual TArray<AActor*> GetArrayAircrafts() override;
	virtual uint8 GetAircraftType() override;
	void DeleteAircraft();
	void SetInitAircraftPosition();
	void SetAircraftFormations();
	void SetTargetAircraft();
	void SetTargetBattleShip();

	virtual void Fire() override;
};
