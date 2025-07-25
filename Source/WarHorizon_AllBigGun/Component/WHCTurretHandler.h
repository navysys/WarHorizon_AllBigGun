#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/WHCustomStructs.h"
#include "Enum/ETurretAttackType.h"
#include "Enum/ETurretState.h"
#include "WHCTurretHandler.generated.h"

struct FTurretArray;
class AWHTurretBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARHORIZON_ALLBIGGUN_API UWHCTurretHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWHCTurretHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void InitTurretHandlerComponent(const TArray<FTurretArray> AllArray, TArray<APawn*>* BattleShips, TArray<APawn*> Aircrafts);
	void SetTargetBattleShips(TArray<APawn*>* BattleShips) { BSTargetArray = BattleShips; }
	void SetTargetAircreafts(const TArray<APawn*>* Aircrafts) { ACTargetArray = *Aircrafts; }

	bool CheckTurretsState(ETurretType TurretsType, ETurretState TurretState);

	void SetMainTurretTarget(AActor* Target);
	void SetMainTurretPoint(FVector Point);
	void CommandTurretsFire(ETurretType TurretType);
protected:
	void SetAirTurretDataToTargetArray();
	void SetSubTurretDataToTargetArray();
	FTargetData CalculateOwnerToPointData(FVector Point);

protected:
	AActor* Owner;
	bool bIsTracingTarget;
	AActor* MTTarget;

	// Ÿ��
	TArray<APawn*>* BSTargetArray;
	TArray<APawn*> ACTargetArray;

	TArray<FTargetData> BSTargetDatas;
	TArray<FTargetData> ACTargetDatas;
	// �ͷ�
	TArray<FTurretArray> AllTurretArray;

	// ���� �ͷ� (��� �߻�� ����)
	TArray<AWHTurretBase*> MainTurrets;
	// ��� �߻��
	TArray<AWHTurretBase*> TorpedoLaunchers;
	// ���� �ͷ�
	TArray<AWHTurretBase*> SubTurrets;
	// ���� ���� ���� �ͷ�
	TArray<AWHTurretBase*> ForwardSTs;
	TArray<AWHTurretBase*> BackSTs;
	TArray<AWHTurretBase*> LeftSTs;
	TArray<AWHTurretBase*> RightSTs;
	// ��� �ͷ�
	TArray<AWHTurretBase*> AirTurrets;
	// ���� ���� ��� �ͷ�
	TArray<AWHTurretBase*> ForwardATs;
	TArray<AWHTurretBase*> BackATs;
	TArray<AWHTurretBase*> LeftATs;
	TArray<AWHTurretBase*> RightATs;
	// ��� �ͷ�
	TArray<AWHTurretBase*> DualTurrets;
	// ���� ���� ��� �ͷ�
	TArray<AWHTurretBase*> ForwardDTs;
	TArray<AWHTurretBase*> BackDTs;
	TArray<AWHTurretBase*> LeftDTs;
	TArray<AWHTurretBase*> RightDTs;



	UPROPERTY(VisibleAnywhere)
	TArray<FTargetData> STFDatas;
	UPROPERTY(VisibleAnywhere)
	TArray<FTargetData> STBDatas;
	UPROPERTY(VisibleAnywhere)
	TArray<FTargetData> STLDatas;
	UPROPERTY(VisibleAnywhere)
	TArray<FTargetData> STRDatas;

	UPROPERTY(VisibleAnywhere)
	FTargetData ATFData;
	UPROPERTY(VisibleAnywhere)
	FTargetData ATBData;
	UPROPERTY(VisibleAnywhere)
	FTargetData ATLData;
	UPROPERTY(VisibleAnywhere)
	FTargetData ATRData;

	FTimerHandle DirectionTimerHandle;
	float DirectionTimerInterval = 0.5f;
};
