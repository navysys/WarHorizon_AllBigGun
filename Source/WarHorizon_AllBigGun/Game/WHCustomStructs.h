// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enum/ETurretAttackType.h"
#include "WHCustomStructs.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API UWHCustomStructs : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(Atomic)
struct FPlayerDataStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString PlayerName;

};

USTRUCT(Atomic)
struct FBattleShipDataStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString PlayerName;
	UPROPERTY(EditAnywhere)
	FString BattleShipName;

	UPROPERTY(EditAnywhere)
	float MaxHP;
	UPROPERTY(EditAnywhere)
	float CurrentHP;
	UPROPERTY(EditAnywhere)
	float MaxMP;
	UPROPERTY(EditAnywhere)
	float CurrentMP;
	UPROPERTY(EditAnywhere)
	float Attack;
	UPROPERTY(EditAnywhere)
	float ReloadSpeed;
	UPROPERTY(EditAnywhere)
	float CooldownSpeed;
	UPROPERTY(EditAnywhere)
	float Defense;
	UPROPERTY(EditAnywhere)
	float Resistance;
	UPROPERTY(EditAnywhere)
	float Penetration;
	UPROPERTY(EditAnywhere)
	float PenetrationCoefficient;

	UPROPERTY(EditAnywhere)
	float MaxMoveSpeed;
	UPROPERTY(EditAnywhere)
	float CurrentMoveSpeed;
	UPROPERTY(EditAnywhere)
	float MoveAcceleration;
	UPROPERTY(EditAnywhere)
	float MoveDeceleration;
	
	UPROPERTY(EditAnywhere)
	float MaxRotSpeed;
	UPROPERTY(EditAnywhere)
	float CurrentRotSpeed;
	UPROPERTY(EditAnywhere)
	float RotAcceleration;
};

USTRUCT(Atomic)
struct FTurretDataStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString PlayerName;
	UPROPERTY(EditAnywhere)
	FString TurretName;
};

USTRUCT(Atomic)
struct FTurretArray
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	ETurretType TurretsType;
	UPROPERTY(EditAnywhere)
	TArray<class AWHTurretBase*> Turrets;
};