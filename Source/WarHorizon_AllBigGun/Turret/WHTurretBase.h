#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/WHCustomStructs.h"
#include "Enum/ETurretType.h"
#include "Enum/ETurretState.h"
#include "WHTurretBase.generated.h"

class UNiagaraSystem;
class UStaticMesh;
class AWHShell;
class UWHCTurretFSM;


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

	void CalculateAngleBetweenTarget();
	void SpinToTargetAngle();

	void SetFrontDirection(char Dir);
	char GetFrontDirection() { return FrontDirection; }
	void SetTargetData(FTargetData Data);
	void SetTargetData(const TArray<FTargetData>* DatasPtr);
	
	ETurretType GetTurretType() { return TurretType; }
	ETurretState GetTurretState();
	bool GetIsLoaded() { return bIsLoaded; }
	bool GetIsAimed() { return bIsAimed; }
	bool GetIsFireReady() { return bIsFireReady; }

	void SetReloadTime(float RTime);
	float GetReloadTime() { return ReloadTime; }

	void SetIsLoaded(bool Loaded);

	void SetFSMCommandState(ETurretState State);

protected:
	void LoadDataTableToName(FName Name);
	void DebugTurretForward();
	

protected:
	UPROPERTY()
	APawn* BaseBattleShip;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> GunFireEffect;

	UPROPERTY(EditAnywhere)
	TArray<USceneComponent*> MuzzleComps;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWHCTurretFSM> FSMComps;


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

	// 포탑의 정면을 LRFB 로 구분
	char FrontDirection;
	float SocketYaw;

	float BeforeFireTime = 0.0f;
	bool bIsLoaded = true;
	bool bIsAimed = false;
	bool bIsFireReady = false;

	float SpinAcceleration = 0.0f;

public: 
	float Angle = 0.0f;
	float TurretYaw = 0.0f;
};
