// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
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
	class IBattleShipInterface* PawnBattleShip;



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
	TObjectPtr<class UInputAction> MoveOrAttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> AccelerationAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> DecelerationAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ChangeContextAction;

	// 
	void MoveOrAttack(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	void Acceleration(const FInputActionValue& Value);

	void Deceleration(const FInputActionValue& Value);

	void ChangeContext(const FInputActionValue& Value);
};
