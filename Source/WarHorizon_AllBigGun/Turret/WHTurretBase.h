// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enum/ETurretType.h"
#include "Engine/StaticMeshSocket.h"
#include "WHTurretBase.generated.h"

class UNiagaraSystem;
class UStaticMesh;

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHTurretBase : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWHTurretBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	void SetFrontDirection(char Dir);
	char GetFrontDirection() { return FrontDirection; }
	void SetupTurret(APawn* BaseShip, FName Name);
	void Fire();
protected:
	void LoadDataTableToName(FName Name);

public:

protected:
	UPROPERTY(VisibleAnywhere)
	APawn* BaseBattleShip;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshSocket*> Muzzles;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraSystem> GunFireEffect;

	UPROPERTY(VisibleAnywhere)
	uint16 TurretID;
	UPROPERTY(VisibleAnywhere)
	FString TurretName;
	UPROPERTY(VisibleAnywhere)
	ETurretType TurretType;

	UPROPERTY(EditAnywhere)
	UStaticMesh* TurretMesh;

	// 포탑의 정면을 LRFB 로 구분
	char FrontDirection;
	UPROPERTY(EditAnywhere)
	float SocketYaw;

	UPROPERTY(EditAnywhere)
	float ReloadTime;
	UPROPERTY(EditAnywhere)
	float Range;

	UPROPERTY(EditAnywhere)
	float MaxHorizontalAngle;
	UPROPERTY(EditAnywhere)
	float MaxVerticalAngle;

	UPROPERTY(EditAnywhere)
	float RotationSpeed;

};
