#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/ETeamType.h"
#include "FOW_Base.generated.h"

class AFOW_Obstacle;

UENUM()
enum class ETileState : uint8
{
	Invisible,
	Visible,
	Obstacle
};


class FGridColumn
{
public:
	FGridColumn(TArray<ETileState>& NewColumn)
	{
		GridColumn = NewColumn;
	};
	ETileState& operator[](int Index)
	{
		if (GridColumn.IsValidIndex(Index))
		{
			return GridColumn[Index];
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GridColumn : Index out of bounds"));
			return GridColumn[0];
		}
	}

public:
	TArray<ETileState> GridColumn;
};

class FGridData
{
public:
	void AddColumn(const FGridColumn& NewColumn)
	{
		GridRow.Add(NewColumn);
	}
	FGridColumn& operator[](int Index)
	{
		if (GridRow.IsValidIndex(Index))
		{
			return GridRow[Index];
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GridRow : Index out of bounds"));
			return GridRow[0];
		}
	}
	void SetTileStateAt(int X, int Y, ETileState NewState)
	{
		if (GridRow.IsValidIndex(Y))
		{
			FGridColumn& Column = GridRow[Y];
			if (Column.GridColumn.IsValidIndex(X))
			{
				Column.GridColumn[X] = NewState;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("SetTileStateAt: X Index out of bounds"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SetTileStateAt: Y Index out of bounds"));
		}
	}

private:
	TArray<FGridColumn> GridRow;
};

UCLASS()
class UFogRevealer : public UObject
{
	GENERATED_BODY()

public:
	UFogRevealer() {};
	UFogRevealer(AActor* RevealerActor, int Range)
	{
		Revealer = RevealerActor;
		SightRange = Range;
	}

	FVector GetCurrentLocation()
	{
		if (IsValid(Revealer))
		{
			return Revealer->GetActorLocation();
		}

		UE_LOG(LogTemp, Warning, TEXT("Revealer Not Found"));
		return FVector::ZeroVector;
	}

	AActor* Revealer;
	int SightRange = 500;
	bool bIsExposedToEnemy = false;
};

UCLASS()
class WARHORIZON_ALLBIGGUN_API AFOW_Base : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFOW_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AddBattleShipRevealer(AActor* NewFogActor, int Range, ETeamType Team);
	virtual void AddAircraftRevealer(AActor* NewFogActor, int Range, ETeamType Team);
	virtual void RemoveBattleShipRevealer(AActor* NewFogActor, ETeamType Team);
	virtual void RemoveAircraftRevealer(AActor* NewFogActor, ETeamType Team);
	void RegisterFOWObstacle(AFOW_Obstacle* Obstacle);
	void RemoveFOWObstacle(AFOW_Obstacle* Obstacle);
protected:
	void InitGrid();
	void ClearGrid();
	virtual void UpdateGrid() {};

	FIntPoint GetGridIndexToWorldLocation(const FVector& WorldLocation);
	void ComputeFogOfWarVisibility(TArray<UFogRevealer*> BattleShipRevealers, TArray<UFogRevealer*> AircraftRevealers);
	void ComputeFOV_BFS(FGridData& Grid, int GridSizeX, int GridSizeY, const FIntPoint& StartIndex, int ViewRadius);

public:
	UPROPERTY(BlueprintReadWrite, Category = "Fog Of War")
	float RefreshRate = 0.2f;

	// Grid settings
	UPROPERTY(BlueprintReadWrite, Category = "Fog Of War")
	int GridExtent = 100000;

	UPROPERTY(BlueprintReadWrite, Category = "Fog Of War")
	int TileSize = 200;

	UPROPERTY()
	int GridSize;

	FGridData* GridDataPtr;
	TSet<FIntPoint> LineTraceQueue;

	FTimerHandle FOVTimerHandle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<UFogRevealer*> BattleShipRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<UFogRevealer*> AircraftRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<AFOW_Obstacle*> AllFogObstacle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TSet<UFogRevealer*> BFSRevealers;
};