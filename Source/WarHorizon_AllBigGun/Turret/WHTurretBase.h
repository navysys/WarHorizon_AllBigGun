#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/WHCustomStructs.h"
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
	virtual void PostInitializeComponents() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Fire();

	void SetFrontDirection(char Dir);
	char GetFrontDirection() { return FrontDirection; }
	void SetTargetData(FTargetData Data);
	void SetTargetData(const TArray<FTargetData>* DatasPtr);


protected:
	void LoadDataTableToName(FName Name);
	void DebugTurretForward();
	void SpinToTargetAngle();

protected:
	UPROPERTY()
	APawn* BaseBattleShip;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	TArray<USceneComponent*> MuzzleComps;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> GunFireEffect;

	// 데이터 베이스 초기화 변수
protected:
	UPROPERTY(EditAnywhere)
	uint16 TurretID;
	UPROPERTY(EditAnywhere)
	FString TurretName;
	UPROPERTY(EditAnywhere)
	ETurretType TurretType;

	UPROPERTY(EditAnywhere)
	float ReloadTime = 8.0f;
	UPROPERTY(EditAnywhere)
	float Range;

	UPROPERTY(EditAnywhere)
	float MaxHorizontalAngle = 150.0f;
	UPROPERTY(EditAnywhere)
	float MaxVerticalAngle;

	UPROPERTY(EditAnywhere)
	float RotationDelay = 0.05f;

	// 포탑 회전을 위한 변수
protected:
	FTimerHandle RotationTimerHandle;

	UPROPERTY(EditAnywhere)
	FTargetData TargetData;

	UPROPERTY(EditAnywhere)
	bool bIsLookTarget = false;

	// 포탑의 정면을 LRFB 로 구분
	char FrontDirection;
	float SocketYaw;

	float BeforeFireTime = 0.0f;

	bool bIsFireReady = false;
};
