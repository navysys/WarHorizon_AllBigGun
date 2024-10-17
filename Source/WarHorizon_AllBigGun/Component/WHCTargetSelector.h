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

	//테스트 함수
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

	// 타겟
	TArray<APawn*> BSTargetArray;
	TArray<APawn*> ACTargetArray;

	TArray<float> ACTargetAngles;
	// 터렛
	TArray<FTurretArray> AllTurretArray;

	// 메인 터렛 (어뢰 발사기 가능)
	TArray<AWHTurretBase*> MainTurrets;
	// 방향 기준 서브 터렛
	TArray<AWHTurretBase*> ForwardSTs;
	TArray<AWHTurretBase*> BackSTs;
	TArray<AWHTurretBase*> LeftSTs;
	TArray<AWHTurretBase*> RightSTs;
	// 방향 기준 대공 터렛
	TArray<AWHTurretBase*> ForwardATs;
	TArray<AWHTurretBase*> BackATs;
	TArray<AWHTurretBase*> LeftATs;
	TArray<AWHTurretBase*> RightATs;
	// 방향 기준 양용 터렛
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
