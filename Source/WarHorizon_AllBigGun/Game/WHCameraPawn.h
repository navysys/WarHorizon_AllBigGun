// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WHCameraPawn.generated.h"

class AWHBattleShipBase;


UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWHCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void SpawnBattleShip();

protected:
	UPROPERTY()
	TObjectPtr<class AWHPlayerController> PlayerController;
	UPROPERTY()
	TObjectPtr<AWHBattleShipBase> PlayerBattleShip;

	UPROPERTY()
	TObjectPtr<UBlueprint> BPBattleShip;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY()
	FVector2D ScreenEdgePadding;

	UPROPERTY()
	FVector2D MoveSpeed;

	FVector2D CurrentInputMoveSpeed;

public:

};
