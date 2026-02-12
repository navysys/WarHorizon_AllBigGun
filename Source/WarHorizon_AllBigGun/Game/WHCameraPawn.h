// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WHCameraPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;


	UPROPERTY(EditAnywhere, Category = "Camera Pawn")
	FVector2D ScreenEdgePadding;

	UPROPERTY(EditAnywhere, Category = "Camera Pawn")
	FVector2D MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Camera Pawn")
	FVector2D CurrentInputMoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Camera Pawn")
	bool bCanCameraMove = true;
};
