#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/WHCustomStructs.h"
#include "Enum/ETurretAttackType.h"
#include "WHCTargetSelector.generated.h"

struct FTurretArray;
class AWHTurretBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARHORIZON_ALLBIGGUN_API UWHCTargetSelector : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWHCTargetSelector();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void InitTargetSelectorComponent(const TArray<FTurretArray> AllArray, TArray<APawn*>* BattleShips, TArray<APawn*> Aircrafts);
	void SetTargetBattleShips(TArray<APawn*>* BattleShips) { BSTargetArray = BattleShips; }
	void SetTargetAircreafts(const TArray<APawn*>* Aircrafts) { ACTargetArray = *Aircrafts; }

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
	// 방향 기준 서브 터렛
	TArray<AWHTurretBase*> ForwardSTs;
	TArray<AWHTurretBase*> BackSTs;
	TArray<AWHTurretBase*> LeftSTs;
	TArray<AWHTurretBase*> RightSTs;
	// 방향 기준 대공 터렛
	TArray<AWHTurretBase*> ForwardATs;
	TArray<AWHTurretBase*> BackATs;
	TArray<AWHTurretBase*> LeftATs;
	TArray<AWHTurretBase*> RightATs;
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
