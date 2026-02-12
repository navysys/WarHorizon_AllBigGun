// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/ETeamType.h"
#include "FogOfWar/FOW_Base.h"
#include "FOW_Manager.generated.h"


class AFogObstacle;

UCLASS()
class WARHORIZON_ALLBIGGUN_API AFOW_Manager : public AFOW_Base
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFOW_Manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AddBattleShipRevealer(AActor* NewFogActor, int Range, ETeamType Team) override;
	virtual void AddAircraftRevealer(AActor* NewFogActor, int Range, ETeamType Team) override;
	virtual void RemoveBattleShipRevealer(AActor* NewFogActor, ETeamType Team) override;
	virtual void RemoveAircraftRevealer(AActor* NewFogActor, ETeamType Team) override;
protected:
	virtual void UpdateGrid() override;
	FIntPoint GetGridIndexToWorldLocation(const FVector& WorldLocation);

	void ComputeRevealerIsExposed(TArray<UFogRevealer*> TeamBattleShips, TArray<UFogRevealer*> TeamAircratfs, TArray<UFogRevealer*> EnemyBattleShips, TArray<UFogRevealer*> EnemyAircreafts);

public:
	// Fog settings
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<UFogRevealer*> RedTeamFogRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<UFogRevealer*> BlueTeamFogRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<UFogRevealer*> RedTeamAircraftRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<UFogRevealer*> BlueTeamAircraftRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<UFogRevealer*> RedTeamExposedRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<UFogRevealer*> BlueTeamExposedRevealers;




};
