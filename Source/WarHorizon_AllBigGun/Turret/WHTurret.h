// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret/WHTurretBase.h"
#include "Enum/ETurretAttackType.h"
#include "Interface/TurretInterface.h"
#include "WHTurret.generated.h"

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHTurret : public AWHTurretBase, public ITurretInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWHTurret();


protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	void SetAttackType(ETurretAttackType Type) { AttackType = Type; }
	void SetTargetAngle(float Angle) { TargetAngle = Angle; }
	void SetTargetPoint(FVector Point) { TargetPoint = Point; }
	void SetTargetPawn(APawn* Pawn) { TargetPawn = Pawn; }
	void SetTargetDistance(float Dist) { TargetDistance = Dist; }

	virtual float GetMaxHorizontalAngle() override;
	virtual APawn* GetBaseBattleShip() override;
	virtual uint8 GetAttackType() override;
	virtual float GetTurretTargetAngle() override;
	virtual APawn* GetTurretTargetPawn() override;
	virtual FVector GetTurretTargetPoint() override;
	virtual float GetRotationSpeed() override;
	virtual float GetReloadTime() override;
	virtual void Fire() override;

public:
	class AWHPlayerController* PlayerController;

protected:
	UPROPERTY(EditAnywhere)
	TArray<float> Dispersion;

	UPROPERTY(VisibleAnywhere)
	float MaxRotationAngle;

	UPROPERTY(VisibleAnywhere)
	ETurretAttackType AttackType = ETurretAttackType::Wait;

	UPROPERTY(VisibleAnywhere)
	float TargetAngle;
	UPROPERTY(VisibleAnywhere)
	APawn* TargetPawn;
	UPROPERTY(VisibleAnywhere)
	FVector TargetPoint;
	UPROPERTY(VisibleAnywhere)
	float TargetDistance;

	UPROPERTY(VisibleAnywhere)
	FVector ShellVelocityVector = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere)
	float ShellVelocity = 0.0f;
	UPROPERTY(EditAnywhere)
	float ShellLaunchAngle;

};
