#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enum/ETeamType.h"
#include "Enum/ERevealerType.h"
#include "FOW_Base.generated.h"

class AFOW_RevealerPawn;
class AFOW_Obstacle;

UENUM()
enum class ETileState : uint8
{
	Invisible,
	Visible,
	Obstacle,
	Side
};


//class FGridColumn
//{
//public:
//	FGridColumn(TArray<ETileState>& NewColumn)
//	{
//		GridColumn = NewColumn;
//	};
//	ETileState& operator[](int Index)
//	{
//		if (GridColumn.IsValidIndex(Index))
//		{
//			return GridColumn[Index];
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("GridColumn : Index out of bounds"));
//			return GridColumn[0];
//		}
//	}
//
//public:
//	TArray<ETileState> GridColumn;
//};
//
//class FGridData
//{
//public:
//	void AddColumn(const FGridColumn& NewColumn)
//	{
//		GridRow.Add(NewColumn);
//	}
//	FGridColumn& operator[](int Index)
//	{
//		if (GridRow.IsValidIndex(Index))
//		{
//			return GridRow[Index];
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("GridRow : Index out of bounds"));
//			return GridRow[0];
//		}
//	}
//	void SetTileStateAt(int X, int Y, ETileState NewState)
//	{
//		if (GridRow.IsValidIndex(Y))
//		{
//			FGridColumn& Column = GridRow[Y];
//			if (Column.GridColumn.IsValidIndex(X))
//			{
//				Column.GridColumn[X] = NewState;
//			}
//			else
//			{
//				UE_LOG(LogTemp, Warning, TEXT("SetTileStateAt: X Index out of bounds - X : %i"), X);
//			}
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("SetTileStateAt: Y Index out of bounds - Y : %i"), Y);
//		}
//	}
//
//private:
//	TArray<FGridColumn> GridRow;
//};

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

	virtual void AddRevealer(AFOW_RevealerPawn* RevealerPawn);
	virtual void RemoveRevealer(AFOW_RevealerPawn* NewFogActor);
	void RegisterFOWObstacle(AFOW_Obstacle* Obstacle);
	void RemoveFOWObstacle(AFOW_Obstacle* Obstacle);
protected:
	virtual void UpdateGrid() {};

	FIntPoint GetGridIndexToWorldLocation(const FVector& WorldLocation);
	void ComputeFogOfWarVisibility(TArray<AFOW_RevealerPawn*> BattleShipRevealers, TArray<AFOW_RevealerPawn*> AircraftRevealers);
	void ComputeFOV_BFS(TArray<uint8>& Grid, int GridSizeX, int GridSizeY, const FIntPoint& StartIndex, int ViewRadius);

	void UpdateSideGrid();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Fog Of War")
	float RefreshRate = 0.2f;

	// Grid settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Fog Of War")
	int GridExtent = 100000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Fog Of War")
	int TileSize = 200;

	UPROPERTY()
	int GridSize = 500;

	TArray<uint8> GridDataArray;
	TSet<FIntPoint> LineTraceQueue;
	TArray<FIntPoint> ObstacleCoordinates;

	FTimerHandle FOVTimerHandle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<AFOW_RevealerPawn*> BattleShipRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<AFOW_RevealerPawn*> AircraftRevealers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TArray<AFOW_Obstacle*> AllFogObstacle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TSet<AFOW_RevealerPawn*> BFSRevealers;

	const TArray<FIntPoint> Directions =
	{
		{ 1,  0}, {-1,  0},
		{ 0,  1}, { 0, -1}
	};
};