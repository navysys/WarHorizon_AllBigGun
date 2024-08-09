// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/BattleShipInterface.h"
#include "Enum/EBattleShipType.h"
#include "Game/WHCustomStructs.h"
#include "WHBattleShip.generated.h"

class AWHTurretBase;
class AWHTurret;

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHBattleShip : public APawn, public IBattleShipInterface
{
	GENERATED_BODY()

public:
	AWHBattleShip();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 데이터 로드 및 초기화 함수
	void LoadSingletonData();
	void LoadDataTableToName(FName Name);
	void CreateTurretToMeshCompSocket(USkeletalMeshComponent* MeshComp, FName BattleShipName);

	// 적 탐지 관련 함수
	void CalculateAngleToSpinTurret();

	//void SetDead();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 인터페이스 관련 함수
	virtual void UserFastFire() override;
	virtual void UserAttack() override;
	virtual void UserAttackCancel() override;
	virtual void UserSpinTurrets(float Angle, float Distance) override;
	virtual void UserSpinTurretsToPawn(APawn* Target) override;
	virtual void UserSpinTurretsToHitPoint(FVector HitPoint) override;
	virtual void UserSkill(char Key) override;
	virtual void CalculateRotationToHitPoint(FVector HitPoint) override;
	virtual void IncreaseMoveSpeed() override;
	virtual void DecreaseMoveSpeed() override;
	virtual void SpawnAircrafts(int Index) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	uint32 ID = 0;
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	FString PlayerName;
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	FString BattleShipName;
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	EBattleShipType BattleShipType = EBattleShipType::Invalid;
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	uint8 TeamInt = 1;


	// Stat
protected:
	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float InitMaxHP;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float HP;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float InitMaxMP;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float MP;


	// 움직임, 회전 관련 변수
protected:
	UPROPERTY(EditAnywhere, Category = "BattleShip | Movement")
	float InitMaxMoveSpeed;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Movement")
	float InitAcceleration;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Movement")
	float InitDeceleration;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Rotation")
	float InitMaxRotationSpeed;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Rotation")
	float InitRotationAcceleration;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Rotation")
	float InitRotationAccelerationIncrease;

protected:
	bool bIsMouseTarget;
	APawn* MouseTarget;
	float MouseTargetDistance;



	// 기본 구성 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USkeletalMeshComponent> SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWHCBattleShipMovement> BattleShipMovementComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWHCDetectEnemy> DetectEnemyComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWHCSkillHandler> SkillHandlerComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWHCTargetSelector> TargetSelectorComp;


	// HP UI
protected:

	// 소켓을 포함하는 선체 매시
	UPROPERTY(EditAnywhere, Category = "BattleShip | Mesh")
	TObjectPtr<class USkeletalMesh> BaseMesh;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Skill")
	TSoftObjectPtr<UObject> SkillPtrQ;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Skill")
	TSoftObjectPtr<UObject> SkillPtrW;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Skill")
	TSoftObjectPtr<UObject> SkillPtrE;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Skill")
	TSoftObjectPtr<UObject> SkillPtrR;

protected:
	// 모든 터렛을 저장
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<FTurretArray> AllTurretArray;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Aircraft")
	TArray<APawn*> AllAircraftsArray;

	//UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	//FTurretArray MainTurrets1;
	//UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	//FTurretArray MainTurrets2;

	//UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	//FTurretArray TorpedoLaunchers1;
	//UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	//FTurretArray TorpedoLaunchers2;

	//UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	//FTurretArray SubTurrets1;
	//UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	//FTurretArray SubTurrets2;

	//UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	//FTurretArray AirTurrets1;
	//UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	//FTurretArray AirTurrets2;

	//UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	//FTurretArray DualTurrets1;
	//UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	//FTurretArray DualTurrets2;

protected:
	UPROPERTY(EditAnywhere, Category = "BattleShip | Target")
	TArray<APawn*> EnemyBattleShips;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Target")
	TArray<APawn*> EnemyAircrafts;
};
