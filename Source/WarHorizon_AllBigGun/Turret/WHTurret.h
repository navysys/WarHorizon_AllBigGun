// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enum/ETurretType.h"
#include "Enum/ETurretAttackType.h"
#include "Interface/TurretInterface.h"
#include "WHTurret.generated.h"

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHTurret : public APawn, public ITurretInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWHTurret();


protected:
	virtual void BeginPlay() override;

	// 데이터 
	void LoadDataTableToName(FName Name);
	void InitStat();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupTurret(APawn* BaseShip, FName Name);

	void SetAttackType(ETurretAttackType Type) { AttackType = Type; }
	void SetTargetAngle(float Angle) { TargetAngle = Angle; }
	void SetTargetPoint(FVector Point) { TargetPoint = Point; }
	void SetTargetPawn(APawn* Pawn) { TargetPawn = Pawn; }
	void SetMuzzles(UStaticMeshComponent* MeshComp);

	float GetHorizontalAngle() { return HorizontalAngle; }
	virtual APawn* GetBaseBattleShip() override;
	virtual uint8 GetAttackType() override;
	virtual float GetTurretTargetAngle() override;
	virtual APawn* GetTurretTargetPawn() override;
	virtual FVector GetTurretTargetPoint() override;
	virtual float GetRotationSpeed() override;
	virtual float GetReloadTime() override;
	virtual uint16 GetAmmo() override;
	virtual void Fire() override;

public:
	UPROPERTY(VisibleAnywhere)
	APawn* BaseBattleShip;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;



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
	uint16 TurretID;
	UPROPERTY(VisibleAnywhere)
	FString TurretName;
	UPROPERTY(VisibleAnywhere)
	ETurretType TurretType;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* TurretMesh;

	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshSocket*> Muzzles;

	UPROPERTY(EditAnywhere)
	uint16 MaxAmmo;
	UPROPERTY(EditAnywhere)
	uint16 Ammo;
	UPROPERTY(EditAnywhere)
	float ReloadTime;
	UPROPERTY(EditAnywhere)
	float Range;

	UPROPERTY(EditAnywhere)
	float HorizontalAngle;
	UPROPERTY(EditAnywhere)
	float VerticalAngle;

	UPROPERTY(EditAnywhere)
	float RotationSpeed;

};
