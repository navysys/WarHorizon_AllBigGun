// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/BattleShipInterface.h"
#include "Enum/EBattleShipType.h"
#include "Game/WHCustomStructs.h"
#include "WHBattleShipBase.generated.h"

class UBoxComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class UWHCBattleShipMovement;
class UWHCDetectEnemy;
class UWHCSkillHandler;
class UWHCTargetSelector;
class USkeletalMesh;
class AWHTurretBase;
class AWHAircraftsBase;
class UWHSkillBase;
class UWHCCameraBodyComponent;

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHBattleShipBase : public APawn, public IBattleShipInterface
{
	GENERATED_BODY()

public:
	AWHBattleShipBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 데이터 로드 및 초기화 함수
	void LoadDataTableToName(FName Name);
	void CreateTurretToMeshCompSocket(USkeletalMeshComponent* MeshComp);

	//void SetDead();

	// 인터페이스 관련 함수
	virtual void RapidAttack() override;
	virtual void NormalAttack() override;
	virtual void SpinTurrets(AActor* Target) override;
	virtual void SpinTurrets(FVector HitPoint) override;
	virtual void UseSkill(char Key) override;
	virtual void SpinBattleShip(FVector HitPoint) override;
	virtual void IncreaseMoveSpeed() override;
	virtual void DecreaseMoveSpeed() override;
	virtual void SpawnAircrafts(int Index) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	uint32 ID = 0;
	UPROPERTY(EditAnywhere, Category = "BattleShip | ID")
	FString BattleShipName = TEXT("Yamato");
	UPROPERTY(EditAnywhere, Category = "BattleShip | ID")
	EBattleShipType BattleShipType = EBattleShipType::Invalid;
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	uint8 TeamInt = 1;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Collsion")
	FVector BoxSize = FVector(8500.0f, 1300.0f, 1500.0f);


	// 움직임, 회전 관련 변수
protected:
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Movement")
	float InitMaxMoveSpeed;

	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Movement")
	float InitAcceleration;

	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Movement")
	float InitDeceleration;

	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Rotation")
	float InitMaxRotationSpeed;

	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Rotation")
	float InitRotationAcceleration;

	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Rotation")
	float InitRotationAccelerationIncrease;

protected:
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Aircraft")
	TArray<int> AircraftIDs;

protected:
	bool bIsMouseTarget;
	AActor* MouseTarget;
	float MouseTargetDistance;



	// 기본 구성 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWHCCameraBodyComponent> CameraBodyComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWHCBattleShipMovement> BattleShipMovementComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWHCDetectEnemy> DetectEnemyComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWHCSkillHandler> SkillHandlerComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWHCTargetSelector> TargetSelectorComp;
protected:

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
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Turret")
	TArray<FTurretArray> AllTurretArray;

	// 모든 항공기를 저장
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Aircraft")
	TArray<AWHAircraftsBase*> AllAircraftsArray;

protected:
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Target")
	TArray<APawn*> EnemyBattleShips;

	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Target")
	TArray<APawn*> EnemyAircrafts;

public:
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turret")
	TSubclassOf<AWHTurretBase> MainTurret;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turret")
	TSubclassOf<AWHTurretBase> SubTurret1;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turret")
	TSubclassOf<AWHTurretBase> SubTurret2;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turret")
	TSubclassOf<AWHTurretBase> AirTurret1;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turret")
	TSubclassOf<AWHTurretBase> AirTurret2;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turret")
	TSubclassOf<AWHTurretBase> DualTurret1;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turret")
	TSubclassOf<AWHTurretBase> DualTurret2;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turret")
	TSubclassOf<AWHTurretBase> TorpedoLauncher;
};
