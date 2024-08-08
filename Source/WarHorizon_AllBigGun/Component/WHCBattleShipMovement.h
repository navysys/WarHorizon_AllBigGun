// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "WHCBattleShipMovement.generated.h"

/**
 * 
 */

UENUM()
enum class ESpeedType : uint8
{
	Back = 0 UMETA(DisplayName = "Back"),
	Stop UMETA(DisplayName = "Stop"),
	OneQuater UMETA(DisplayName = "1/4 Max"),
	OneHalf UMETA(DisplayName = "2/4 Max"),
	ThreeQuater UMETA(DisplayName = "3/4 Max"),
	Max UMETA(DisplayName = "Max"),
	Invalid
};

UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class WARHORIZON_ALLBIGGUN_API UWHCBattleShipMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()
	
public:
	UWHCBattleShipMovement();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bIsMoveForward = true;
	bool bIsReverseDirection = false;
public:
	// 앞뒤
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	bool bCanChangeType = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	ESpeedType SpeedType = ESpeedType::Stop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	float InitMaxSpeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	float CurrentSpeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	float NormalizedSpeed = 0;

	// 회전
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	bool bCanRotation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	float TurnAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	float RotationAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	float MaxRotationSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	float InitMaxRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	float InitRotationAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WHCBattleShipMovement)
	float RotationAccelerationIncrease;

	bool bReverseDirection;
	bool bIsRotationDeceleration;
	bool bIsTurnLeft;

public:

	void InitBattleShipMovementComponent(float InitMaxMoveSpeed, float InitAcceleration, float InitDeceleration, float InitMaxRotSpeed, float InitRotAcceleration, float InitRotationAccelerationIncrease);
	// 앞뒤
	void SetInitMaxSpeed(float Speed) { InitMaxSpeed = Speed; }
	void SetMaxSpeedToSpeedType();
	void SetSpeedType(uint8 TypeNum);
	void IncreaseSpeedType();
	void DecreaseSpeedType();

	//회전
	void ChangeRotation(float DeltaTime);
	void CalculateAngle(FVector HitPoint);
};
