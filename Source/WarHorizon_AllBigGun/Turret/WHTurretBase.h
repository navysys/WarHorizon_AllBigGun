// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/ETurretType.h"
#include "WHTurretBase.generated.h"

class UNiagaraSystem;
class UStaticMesh;
class AWHShell;

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHTurretBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWHTurretBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

public:	
	void SetFrontDirection(char Dir);
	char GetFrontDirection() { return FrontDirection; }
	virtual void Fire();
protected:
	void LoadDataTableToName(FName Name);
	void DebugTurretForward();
	bool SpinToTargetAngle();

public:
	void SetTargetDistance(float Distance);
	void SetTargetAngle(float Angle);


protected:
	UPROPERTY(VisibleAnywhere)
	APawn* BaseBattleShip;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	TArray<USceneComponent*> MuzzleComps;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraSystem> GunFireEffect;

	UPROPERTY(VisibleAnywhere)
	uint16 TurretID;
	UPROPERTY(VisibleAnywhere)
	FString TurretName;
	UPROPERTY(VisibleAnywhere)
	ETurretType TurretType;

	// ��ž�� ������ LRFB �� ����
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

	UPROPERTY(EditAnywhere)
	float TargetAngle = 0.0f;

	UPROPERTY(EditAnywhere)
	bool bIsLookTarget = true;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWHShell> Shell;

	UPROPERTY(VisibleAnywhere)
	float TargetDistance;

	UPROPERTY(EditAnywhere)
	TArray<float> Dispersion;

	// ��ź ���ư��� ������ ���� ����
	// Air �� Torpedo �ͷ������� ��� x
	UPROPERTY(VisibleAnywhere)
	FVector ShellVelocityVector = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere)
	float ShellVelocity = 0.0f;

	UPROPERTY(EditAnywhere)
	float ShellLaunchAngle;

	// �����̺�� Ʈ�� ���
	// 
};
