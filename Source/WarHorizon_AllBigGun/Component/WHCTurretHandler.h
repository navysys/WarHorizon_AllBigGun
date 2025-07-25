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

	// 타겟
	TArray<APawn*>* BSTargetArray;
	TArray<APawn*> ACTargetArray;

	TArray<FTargetData> BSTargetDatas;
	TArray<FTargetData> ACTargetDatas;
	// 터렛
	TArray<FTurretArray> AllTurretArray;

	// 메인 터렛 (어뢰 발사기 포함)
	TArray<AWHTurretBase*> MainTurrets;
	// 어뢰 발사관
	TArray<AWHTurretBase*> TorpedoLaunchers;
	// 서브 터렛
	TArray<AWHTurretBase*> SubTurrets;
	// 방향 기준 서브 터렛
	TArray<AWHTurretBase*> ForwardSTs;
	TArray<AWHTurretBase*> BackSTs;
	TArray<AWHTurretBase*> LeftSTs;
	TArray<AWHTurretBase*> RightSTs;
	// 대공 터렛
	TArray<AWHTurretBase*> AirTurrets;
	// 방향 기준 대공 터렛
	TArray<AWHTurretBase*> ForwardATs;
	TArray<AWHTurretBase*> BackATs;
	TArray<AWHTurretBase*> LeftATs;
	TArray<AWHTurretBase*> RightATs;
	// 양용 터렛
	TArray<AWHTurretBase*> DualTurrets;
	// 방향 기준 양용 터렛
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
