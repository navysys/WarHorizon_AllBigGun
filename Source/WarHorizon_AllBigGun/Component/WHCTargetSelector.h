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

protected:
	void CalculateToTargetInfo();

public:
	void InitTargetSelectorComponent(const TArray<FTurretArray> AllArray, TArray<APawn*> BattleShips, TArray<APawn*> Aircrafts);
	void SetTargetBattleShips(const TArray<APawn*>* BattleShips) { BSTargetArray = *BattleShips; }
	void SetTargetAircreafts(const TArray<APawn*>* Aircrafts) { ACTargetArray = *Aircrafts; }

	//�׽�Ʈ �Լ�
	void SetCurrentLeftAngle(float Left) { CurrentLeftAngle = Left; }

	void SetMainTurretTarget(AActor* Target);
	void SetMainTurretAngleAndDistance(FVector Point);
	void CommandTurretsFire(ETurretType TurretType);
	void UpdateTargetInfo(ETurretType TurretType, APawn* TargetPawn);
	void UpdateTargetInfo(ETurretType TurretType, FVector Point);
protected:
	void CalculateAirTurretRotation();

protected:
	AActor* Owner;
	bool bIsTracingTarget;
	AActor* MainTarget;

	// Ÿ��
	TArray<APawn*> BSTargetArray;
	TArray<APawn*> ACTargetArray;

	TArray<float> ACTargetAngles;
	// �ͷ�
	TArray<FTurretArray> AllTurretArray;

	// ���� �ͷ� (��� �߻�� ����)
	TArray<AWHTurretBase*> MainTurrets;
	// ���� ���� ���� �ͷ�
	TArray<AWHTurretBase*> ForwardSTs;
	TArray<AWHTurretBase*> BackSTs;
	TArray<AWHTurretBase*> LeftSTs;
	TArray<AWHTurretBase*> RightSTs;
	// ���� ���� ��� �ͷ�
	TArray<AWHTurretBase*> ForwardATs;
	TArray<AWHTurretBase*> BackATs;
	TArray<AWHTurretBase*> LeftATs;
	TArray<AWHTurretBase*> RightATs;
	// ���� ���� ��� �ͷ�
	TArray<AWHTurretBase*> ForwardDTs;
	TArray<AWHTurretBase*> BackDTs;
	TArray<AWHTurretBase*> LeftDTs;
	TArray<AWHTurretBase*> RightDTs;

	UPROPERTY(VisibleAnywhere)
	float CurrentFowardAngle = 9999.9999f;
	UPROPERTY(VisibleAnywhere)
	float CurrentBackAngle = 9999.9999f;
	UPROPERTY(VisibleAnywhere)
	float CurrentLeftAngle = 9999.9999f;
	UPROPERTY(VisibleAnywhere)
	float CurrentRightAngle = 9999.9999f;

	FTimerHandle DirectionTimerHandle;
	float DirectionTimerInterval = 0.5f;
};
