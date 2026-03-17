// Fill out your copyright notice in the Description page of Project Settings.


#include "WarHorizon_AllBigGun/FogOfWar/FOW_Manager.h"
#include "WarHorizon_AllBigGun/FogOfWar/FOW_Obstacle.h"
#include "WarHorizon_AllBigGun/FogOfWar/FOW_RevealerPawn.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AFOW_Manager::AFOW_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFOW_Manager::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(FOVTimerHandle, this, &AFOW_Manager::UpdateGrid, 0.2f, true);
	
	//FogMaterialInstance = UMaterialInstanceDynamic::Create(FogMaterial, this);
	//FogMaterialInstance->SetScalarParameterValue(FName("GridExtent"), GridExtent);

	//FogPostProcessVolume->Settings.WeightedBlendables.Array.Empty();
	//FogPostProcessVolume->AddOrUpdateBlendable(FogMaterialInstance);

	//GridSize = GridExtent / TileSize;
	//FogGridTexture = UTexture2D::CreateTransient(GridSize, GridSize, PF_R8);

	//FogGridTexture->CompressionSettings = TC_Grayscale;
	//FogGridTexture->Filter = TF_Nearest;
	//FogGridTexture->SRGB = false;
	//FogGridTexture->AddToRoot();
	//FogGridTexture->UpdateResource();


	// 디버깅용 라인 트레이스
	//int NumTilesPerSide = GridExtent / TileSize;
	//for (int x = 0; x < NumTilesPerSide; x++)
	//{
	//	for (int y = 0; y < NumTilesPerSide; y++)
	//	{

	//		FVector TileWorldPos = FVector((x - NumTilesPerSide * 0.5f + 0.5f) * TileSize, (y - NumTilesPerSide * 0.5f + 0.5f) * TileSize, 0.0f);
	//		FVector TraceStart = TileWorldPos + FVector(0.0f, 0.0f, 300.f);
	//		FVector TraceEnd = TileWorldPos - FVector(0.0f, 0.0f, 300.f);

	//		FHitResult OutHit;
	//		UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel14), true, TArray<AActor*>(), EDrawDebugTrace::ForDuration, OutHit, false);
	//	}
	//}


}

// Called every frame
void AFOW_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

void AFOW_Manager::AddRevealer(AFOW_RevealerPawn* RevealerPawn)
{
	if (IsValid(RevealerPawn))
	{
		BattleShipRevealers.Add(RevealerPawn);
		if (RevealerPawn->TeamType == ETeamType::Red)
		{
			RedTeamFogRevealers.Add(RevealerPawn);
		}
		else if (RevealerPawn->TeamType == ETeamType::Blue)
		{
			BlueTeamFogRevealers.Add(RevealerPawn);
		}
	}


}

void AFOW_Manager::RemoveRevealer(AFOW_RevealerPawn* RevealerPawn)
{
	if (IsValid(RevealerPawn))
	{
		if (RevealerPawn->RevealerType == ERevealerType::BattleShip)
		{
			if (RevealerPawn->TeamType == ETeamType::Red)
			{
				for (int i = 0; i < RedTeamFogRevealers.Num(); i++)
				{
					if (RedTeamFogRevealers[i] == RevealerPawn)
					{
						RedTeamFogRevealers.RemoveAt(i);
						break;
					}
				}
			}
			else if (RevealerPawn->TeamType == ETeamType::Blue)
			{
				for (int i = 0; i < BlueTeamFogRevealers.Num(); i++)
				{
					if (BlueTeamFogRevealers[i] == RevealerPawn)
					{
						BlueTeamFogRevealers.RemoveAt(i);
						break;
					}
				}
			}
		}
		else if (RevealerPawn->RevealerType == ERevealerType::Aircraft)
		{
			if (RevealerPawn->TeamType == ETeamType::Red)
			{
				for (int i = 0; i < RedTeamAircraftRevealers.Num(); i++)
				{
					if (RedTeamAircraftRevealers[i] == RevealerPawn)
					{
						RedTeamAircraftRevealers.RemoveAt(i);
						break;
					}
				}
			}
			else if (RevealerPawn->TeamType == ETeamType::Blue)
			{
				for (int i = 0; i < BlueTeamAircraftRevealers.Num(); i++)
				{
					if (BlueTeamAircraftRevealers[i] == RevealerPawn)
					{
						BlueTeamAircraftRevealers.RemoveAt(i);
						break;
					}
				}
			}
		}
	}

}

void AFOW_Manager::UpdateGrid()
{
	// 시야에 노출된 Revealer 계산
	ComputeRevealerIsExposed(RedTeamFogRevealers, RedTeamAircraftRevealers, BlueTeamFogRevealers, BlueTeamAircraftRevealers);
	ComputeRevealerIsExposed(BlueTeamFogRevealers, BlueTeamAircraftRevealers, RedTeamFogRevealers, RedTeamAircraftRevealers);
}

FIntPoint AFOW_Manager::GetGridIndexToWorldLocation(const FVector& WorldLocation)
{
	GridSize = GridExtent / TileSize;
	int TileX = FMath::FloorToInt(WorldLocation.X / TileSize) + GridSize / 2;
	int TileY = FMath::FloorToInt(WorldLocation.Y / TileSize) + GridSize / 2;

	return FIntPoint(TileX, TileY);
}

bool AFOW_Manager::CheckObstacle(FIntPoint Coord, bool& result)
{
	if (ObstacleCoordinates.Find(FIntPoint(Coord.X, Coord.Y)) == INDEX_NONE)
	{
		if (GridDataArray[Coord.Y * GridSize + Coord.X] == 255)
		{
			result = true;
		}
		else
		{
			result = false;
		}
		return false;
	}
	return true;
}

void AFOW_Manager::ComputeRevealerIsExposed(TArray<AFOW_RevealerPawn*> TeamBattleShips, TArray<AFOW_RevealerPawn*> TeamAircratfs, TArray<AFOW_RevealerPawn*> EnemyBattleShips, TArray<AFOW_RevealerPawn*> EnemyAircreafts)
{
	ComputeFogOfWarVisibility(TeamBattleShips, TeamAircratfs);

	// 그리드 기준으로 시야에 노출된 적팀 유닛 처리
	for (AFOW_RevealerPawn* EnemyBattleShip : EnemyBattleShips)
	{
		FVector RevealerLocation = EnemyBattleShip->GetActorLocation();
		FIntPoint RevealerIndex = GetGridIndexToWorldLocation(RevealerLocation);
		if (GridDataArray[RevealerIndex.Y * GridSize + RevealerIndex.X] == 0)
		{
			// 적 유닛이 시야에 노출됨
			EnemyBattleShip->FOWVisibility = EFOWVisibility::Visible;
		}
		else
		{
			EnemyBattleShip->FOWVisibility = EFOWVisibility::Hidden;
		}
	}

	for (AFOW_RevealerPawn* EnemyAircraft : EnemyAircreafts)
	{
		FVector RevealerLocation = EnemyAircraft->GetActorLocation();
		FIntPoint RevealerIndex = GetGridIndexToWorldLocation(RevealerLocation);
		if (GridDataArray[RevealerIndex.Y * GridSize + RevealerIndex.X] == 0)
		{
			// 적 유닛이 시야에 노출됨
			EnemyAircraft->FOWVisibility = EFOWVisibility::Hidden;
		}
		else if (GridDataArray[RevealerIndex.Y * GridSize + RevealerIndex.X] == 255)
		{
			EnemyAircraft->FOWVisibility = EFOWVisibility::Visible;
		}
		else if (ObstacleCoordinates.Find(FIntPoint(RevealerIndex.X, RevealerIndex.Y)) != INDEX_NONE)
		{
			// 보이는 장애물 위에 있는 지 검사
			bool bIsObstacleVisible;
			int Count = 0;
			while (true)
			{
				Count++;
				if (!CheckObstacle(FIntPoint(RevealerIndex.X + Count, RevealerIndex.Y),  bIsObstacleVisible))
				{
					break;
				}
				else if (!CheckObstacle(FIntPoint(RevealerIndex.X - Count, RevealerIndex.Y),  bIsObstacleVisible))
				{
					break;
				}
				else if (!CheckObstacle(FIntPoint(RevealerIndex.X, RevealerIndex.Y + Count), bIsObstacleVisible))
				{
					break;
				}
				else if (!CheckObstacle(FIntPoint(RevealerIndex.X, RevealerIndex.Y - Count), bIsObstacleVisible))
				{
					break;
				}
			}
			// 제일 가까운 노드 정보
			if (bIsObstacleVisible)
			{
				EnemyAircraft->FOWVisibility = EFOWVisibility::Visible;
			}
			else
			{
				EnemyAircraft->FOWVisibility = EFOWVisibility::Hidden;
			}
		}
	}
}