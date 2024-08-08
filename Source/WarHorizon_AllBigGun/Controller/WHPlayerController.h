// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Game/WHCustomStructs.h"
#include "WHPlayerController.generated.h"

/**
 *
 */


UENUM()
enum class EControllerMappingType : uint8
{
	Default,
	WaitingAttack
};

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWHPlayerController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetupInputComponent() override;

private:
	class IBattleShipInterface* BattleShipPawn;
	class AWHCameraPawn* CameraPawn;


protected:
	EControllerMappingType CurrentControllerMappingType;

	void SetControllerMappingType(EControllerMappingType NewControllerMappingType);

	// IM
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> WaitingAttackContext;

	// IA
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveOrTargetingAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> FastFireAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> SpinTurretAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> AccelerationAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> DecelerationAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ChangeContextAction;


	// 
	void MoveOrTargeting(const FInputActionValue& Value);

	void FastFire(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	void SpinTurret(const FInputActionValue& Value);

	void Acceleration(const FInputActionValue& Value);

	void Deceleration(const FInputActionValue& Value);

	void ChangeContext(const FInputActionValue& Value);

public:

	FBattleShipDataStruct GetBattleShipData();

	void SetBattleShipPawn(class IBattleShipInterface* BattleShipPawn);

public:
	UPROPERTY(EditAnywhere)
	FPlayerDataStruct PlayerData;
	UPROPERTY(EditAnywhere)
	FBattleShipDataStruct BattleShipData;
	UPROPERTY(EditAnywhere)
	FTurretDataStruct TurretData;
	

	float ShellSpeed;
};
