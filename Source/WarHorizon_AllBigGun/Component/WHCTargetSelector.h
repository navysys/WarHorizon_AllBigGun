// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/WHCustomStructs.h"
#include "Enum/ETurretAttackType.h"
#include "WHCTargetSelector.generated.h"

struct FTurretArray;
class AWHTurretBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARHORIZON_ALLBIGGUN_API UWHCTargetSelector : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWHCTargetSelector();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void InitTargetSelectorComponent(const TArray<FTurretArray> AllArray, TArray<APawn*> BattleShips, TArray<APawn*> Aircrafts);
	void SetTargetBattleShips(const TArray<APawn*>* BattleShips) { BSTargetArray = *BattleShips; }
	void SetTargetAircreafts(const TArray<APawn*>* Aircrafts) { ACTargetArray = *Aircrafts; }

	void SetTurretsAttackType(ETurretType TurretType, ETurretAttackType Type);
	void SetTurretsTargetDistance(ETurretType TurretType, float Dist);
	void SetTurretsTarget(ETurretType TurretType, APawn* Pawn);
	void SetTurretsTarget(ETurretType TurretType, FVector Point);
	void SetTurretsTarget(ETurretType TurretType, float Angle);

	//�׽�Ʈ �Լ�
	void SetCurrentLeftAngle(float Left) { CurrentLeftAngle = Left; }

	void CommandTurretsFire(ETurretType TurretType);
protected:
	void CalculateAirTurretRotation();
	void ChangeAirTurretRotation(TArray<AWHTurretBase*> TurretDir, float CurrentAngle, float DeltaTime);

protected:
	// Ÿ��
	TArray<APawn*> BSTargetArray;
	TArray<APawn*> ACTargetArray;

	TArray<float> ACTargetAngles;
	// �ͷ�
	TArray<FTurretArray> AllTurretArray;
	TArray<AWHTurretBase*> AirTurretArray;
	// ���� �� ��� �ͷ�
	TArray<AWHTurretBase*> FowardAttackAirTurrets;
	TArray<AWHTurretBase*> BackAttackAirTurrets;
	TArray<AWHTurretBase*> LeftAttackAirTurrets;
	TArray<AWHTurretBase*> RightAttackAirTurrets;

	UPROPERTY(VisibleAnywhere)
	float CurrentFowardAngle = 99999.0f;
	UPROPERTY(VisibleAnywhere)
	float CurrentBackAngle = 99999.0f;
	UPROPERTY(VisibleAnywhere)
	float CurrentLeftAngle = 99999.0f;
	UPROPERTY(VisibleAnywhere)
	float CurrentRightAngle = 99999.0f;

	FTimerHandle DirectionTimerHandle;
	float DirectionTimerInterval = 0.5f;

	int16 AirTurretArrayNum;
	UPROPERTY(VisibleAnywhere)
	bool IsActiveAirTurrets;

	float AirTurretRotationSpeed;
};
