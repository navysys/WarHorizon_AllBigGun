// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/BattleShipInterface.h"
#include "Enum/EBattleShipType.h"
#include "WHBattleShip.generated.h"


class AWHTurret;


UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHBattleShip : public APawn, public IBattleShipInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWHBattleShip();


protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 움직임, 회전 관련 함수
	void MoveForward();
	void ChangeRotation(float DeltaTime);

	// 데이터 로드 및 초기화 함수
	void LoadDataTableToName(FName Name);
	void CreateTurretToMeshCompSocket(UStaticMeshComponent* MeshComp, FName BattleShipName);
	void InitStat();

	// 적 탐지 관련 함수
	void DetectBattleShip();
	void DetectAircraft();
	void SortingPawnArrayToDistance(TArray<APawn*> ArrayPawn);

	//void SetDead();

	// 포탑에 공격할 목표를 할당
	void SetSubTurretAngle(float Angle);
	void SetSubTurretPoint(FVector Point);
	void SetSubTurretTarget(APawn* Target);

public:
	virtual void Tick(float DeltaTime) override;
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 인터페이스 관련 함수
	virtual void UserAttack() override;
	virtual void UserAttackCancel() override;
	virtual void UserSkill1() override;
	virtual void UserSkill2() override;
	virtual void UserSkill3() override;
	virtual void UserSkill4() override;
	virtual void CalculateRotationToHitPoint(FVector HitPoint) override;
	virtual void IncreaseMoveSpeed() override;
	virtual void DecreaseMoveSpeed() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	uint32 ID = 0;
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	FString BattleShipName = TEXT("None");
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | ID")
	EBattleShipType BattleShipType = EBattleShipType::Invalid;


	// Stat
protected:
	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float MaxHP;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float HP;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float MaxMP;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Stat")
	float MP;


	// 움직임, 회전 관련 변수
protected:
	UPROPERTY(EditAnywhere, Category = "BattleShip | Movement")
	bool bCanMove;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Rotation")
	bool bCanRotation;


	UPROPERTY(EditAnywhere, Category = "BattleShip | Movement")
	float InitMaxMoveSpeed;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Movement")
	float InitAcceleration;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Movement")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Movement")
	float Acceleration;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Movement")
	float Deceleration;


	UPROPERTY(EditAnywhere, Category = "BattleShip | Rotation")
	float TurnAngle;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Rotation")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Rotation")
	float RotationAcceleration;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Rotation")
	float MaxRotationSpeed;


	UPROPERTY(EditAnywhere, Category = "BattleShip | Rotation")
	float InitMaxRotationSpeed;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Rotation")
	float InitRotationAcceleration;

	bool bReverseDirection;
	bool bIsRotationDeceleration;
	bool bIsTurnLeft;

	// 적 탐지 관련
protected:
	FTimerHandle DetectTimerHandle;

	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Attack")
	TArray<APawn*> InRangeShips;
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Attack")
	TObjectPtr<APawn> AttackTargetShip;

	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Attack")
	TArray<APawn*> InRangeAircraft;
	UPROPERTY(VisibleAnywhere, Category = "BattleShip | Attack")
	TObjectPtr<APawn> AttackTargetAircraft;


	// 기본 구성 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UFloatingPawnMovement> FloatingPawnMove;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	// HP UI
protected:



	// 소켓을 포함하는 선체 매시
	UPROPERTY(EditAnywhere, Category = "BattleShip | Mesh")
	class UStaticMesh* BaseMesh;


protected:
	// 모든 터렛을 저장
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<AWHTurret*> AllTurrets;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<AWHTurret*> MainTurrets;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<AWHTurret*> TorpedoLaunchers;


	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<AWHTurret*> SubTurrets1;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<AWHTurret*> SubTurrets2;


	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<AWHTurret*> AirTurrets1;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<AWHTurret*> AirTurrets2;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<AWHTurret*> AirTurrets3;
	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<AWHTurret*> AirTurrets4;


	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<AWHTurret*> DualPurposeTurrets1;

	UPROPERTY(EditAnywhere, Category = "BattleShip | Turrets")
	TArray<AWHTurret*> DualPurposeTurrets2;
};
