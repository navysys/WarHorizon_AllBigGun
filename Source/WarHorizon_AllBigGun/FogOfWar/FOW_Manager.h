// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/ETeamType.h"
#include "FogOfWar/FOW_Base.h"
#include "FOW_Manager.generated.h"

class AFOW_RevealerPawn;
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

	virtual void AddRevealer(AFOW_RevealerPawn* RevealerPawn) override;
	virtual void RemoveRevealer(AFOW_RevealerPawn* RevealerPawn) override;
protected:
	virtual void UpdateGrid() override;
	FIntPoint GetGridIndexToWorldLocation(const FVector& WorldLocation);
	bool CheckObstacle(FIntPoint Coord, bool& result);
	void ComputeRevealerIsExposed(TArray<AFOW_RevealerPawn*> TeamBattleShips, TArray<AFOW_RevealerPawn*> TeamAircratfs, TArray<AFOW_RevealerPawn*> EnemyBattleShips, TArray<AFOW_RevealerPawn*> EnemyAircreafts);

public:
	// Fog settings
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<AFOW_RevealerPawn*> RedTeamFogRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<AFOW_RevealerPawn*> BlueTeamFogRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<AFOW_RevealerPawn*> RedTeamAircraftRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<AFOW_RevealerPawn*> BlueTeamAircraftRevealers;

};
