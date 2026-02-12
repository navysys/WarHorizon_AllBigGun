// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FOW_Manager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FogOfWar/FOW_Obstacle.h"

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
	
	InitGrid();
	UE_LOG(LogTemp, Warning, TEXT("AFogOfWarManager : BeginPlay"));
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

void AFOW_Manager::AddBattleShipRevealer(AActor* NewFogActor, int Range, ETeamType Team)
{
	UFogRevealer* NewRevealer = NewObject<UFogRevealer>();
	NewRevealer->Revealer = NewFogActor;
	NewRevealer->SightRange = Range;

	if (Team == ETeamType::Red)
	{
		RedTeamFogRevealers.Add(NewRevealer);
	}
	else if (Team == ETeamType::Blue)
	{
		BlueTeamFogRevealers.Add(NewRevealer);
	}
}

void AFOW_Manager::AddAircraftRevealer(AActor* NewFogActor, int Range, ETeamType Team)
{
	UFogRevealer* NewRevealer = NewObject<UFogRevealer>();
	NewRevealer->Revealer = NewFogActor;
	NewRevealer->SightRange = Range;
	if (Team == ETeamType::Red)
	{
		RedTeamAircraftRevealers.Add(NewRevealer);
	}
	else if (Team == ETeamType::Blue)
	{
		BlueTeamAircraftRevealers.Add(NewRevealer);
	}
}

void AFOW_Manager::RemoveBattleShipRevealer(AActor* NewFogActor, ETeamType Team)
{
	if (Team == ETeamType::Red)
	{
		for (int i = 0; i < RedTeamFogRevealers.Num(); i++)
		{
			if (RedTeamFogRevealers[i]->Revealer == NewFogActor)
			{
				RedTeamFogRevealers.RemoveAt(i);
				break;
			}
		}
	}
	else if (Team == ETeamType::Blue)
	{
		for (int i = 0; i < BlueTeamFogRevealers.Num(); i++)
		{
			if (BlueTeamFogRevealers[i]->Revealer == NewFogActor)
			{
				BlueTeamFogRevealers.RemoveAt(i);
				break;
			}
		}
	}
}

void AFOW_Manager::RemoveAircraftRevealer(AActor* NewFogActor, ETeamType Team)
{
	if (Team == ETeamType::Red)
	{
		for (int i = 0; i < RedTeamAircraftRevealers.Num(); i++)
		{
			if (RedTeamAircraftRevealers[i]->Revealer == NewFogActor)
			{
				RedTeamAircraftRevealers.RemoveAt(i);
				break;
			}
		}
	}
	else if (Team == ETeamType::Blue)
	{
		for (int i = 0; i < BlueTeamAircraftRevealers.Num(); i++)
		{
			if (BlueTeamAircraftRevealers[i]->Revealer == NewFogActor)
			{
				BlueTeamAircraftRevealers.RemoveAt(i);
				break;
			}
		}
	}
}

void AFOW_Manager::UpdateGrid()
{
	// 시야에 노출된 Revealer 계산
	ComputeRevealerIsExposed(RedTeamFogRevealers, RedTeamAircraftRevealers, BlueTeamFogRevealers, BlueTeamAircraftRevealers);
	ComputeRevealerIsExposed(BlueTeamFogRevealers, BlueTeamAircraftRevealers, RedTeamFogRevealers, RedTeamAircraftRevealers);

	RedTeamExposedRevealers.Empty();
	BlueTeamExposedRevealers.Empty();
	for(UFogRevealer* Revealer : RedTeamFogRevealers)
	{
		if (Revealer->bIsExposedToEnemy)
		{
			RedTeamExposedRevealers.Add(Revealer);
		}
	}
	for (UFogRevealer* Revealer : BlueTeamFogRevealers)
	{
		if (Revealer->bIsExposedToEnemy)
		{
			BlueTeamExposedRevealers.Add(Revealer);
		}
	}
	for(UFogRevealer* Revealer : RedTeamAircraftRevealers)
	{
		if (Revealer->bIsExposedToEnemy)
		{
			RedTeamExposedRevealers.Add(Revealer);
		}
	}
	for (UFogRevealer* Revealer : BlueTeamAircraftRevealers)
	{
		if (Revealer->bIsExposedToEnemy)
		{
			BlueTeamExposedRevealers.Add(Revealer);
		}
	}

	// 여기서 게임모드 함수 호출해서 레드팀에게는 노출된 블루팀, 블루팀에게는 노출된 레드팀 정보를 전달

}

FIntPoint AFOW_Manager::GetGridIndexToWorldLocation(const FVector& WorldLocation)
{
	GridSize = GridExtent / TileSize;
	int TileX = FMath::FloorToInt(WorldLocation.X / TileSize) + GridSize / 2;
	int TileY = FMath::FloorToInt(WorldLocation.Y / TileSize) + GridSize / 2;

	return FIntPoint(TileX, TileY);
}

void AFOW_Manager::ComputeRevealerIsExposed(TArray<UFogRevealer*> TeamBattleShips, TArray<UFogRevealer*> TeamAircratfs, TArray<UFogRevealer*> EnemyBattleShips, TArray<UFogRevealer*> EnemyAircreafts)
{
	ComputeFogOfWarVisibility(TeamBattleShips, TeamAircratfs);

	if (GridDataPtr != nullptr)
	{
		// 그리드 기준으로 시야에 노출된 적팀 유닛 처리
		for (UFogRevealer* EnemyBattleShip : EnemyBattleShips)
		{
			FVector RevealerLocation = EnemyBattleShip->GetCurrentLocation();
			FIntPoint RevealerIndex = GetGridIndexToWorldLocation(RevealerLocation);
			if ((*GridDataPtr)[RevealerIndex.X][RevealerIndex.Y] == ETileState::Invisible)
			{
				// 적 유닛이 시야에 노출됨
				EnemyBattleShip->bIsExposedToEnemy = false;
			}
			else
			{
				EnemyBattleShip->bIsExposedToEnemy = true;
			}
		}

		for (UFogRevealer* EnemyAircraft : EnemyAircreafts)
		{
			FVector RevealerLocation = EnemyAircraft->GetCurrentLocation();
			FIntPoint RevealerIndex = GetGridIndexToWorldLocation(RevealerLocation);
			if ((*GridDataPtr)[RevealerIndex.X][RevealerIndex.Y] == ETileState::Invisible)
			{
				// 적 유닛이 시야에 노출됨
				EnemyAircraft->bIsExposedToEnemy = false;
			}
			else if ((*GridDataPtr)[RevealerIndex.X][RevealerIndex.Y] == ETileState::Visible)
			{
				EnemyAircraft->bIsExposedToEnemy = true;
			}
			else if ((*GridDataPtr)[RevealerIndex.X][RevealerIndex.Y] == ETileState::Obstacle)
			{
				// 보이는 장애물 위에 있는 지 검사
				bool bIsObstacleVisible;
				int Count = 0;
				while (true)
				{
					Count++;
					if ((*GridDataPtr)[RevealerIndex.X + Count][RevealerIndex.Y] != ETileState::Obstacle)
					{
						if ((*GridDataPtr)[RevealerIndex.X + Count][RevealerIndex.Y] == ETileState::Visible)
						{
							bIsObstacleVisible = true;
						}
						else
						{
							bIsObstacleVisible = false;
						}
						break;
					}
					else if ((*GridDataPtr)[RevealerIndex.X - Count][RevealerIndex.Y] != ETileState::Obstacle)
					{
						if ((*GridDataPtr)[RevealerIndex.X - Count][RevealerIndex.Y] == ETileState::Visible)
						{
							bIsObstacleVisible = true;
						}
						else
						{
							bIsObstacleVisible = false;
						}
						break;
					}
					else if ((*GridDataPtr)[RevealerIndex.X][RevealerIndex.Y + Count] != ETileState::Obstacle)
					{
						if ((*GridDataPtr)[RevealerIndex.X][RevealerIndex.Y + Count] == ETileState::Visible)
						{
							bIsObstacleVisible = true;
						}
						else
						{
							bIsObstacleVisible = false;
						}
						break;
					}
					else if ((*GridDataPtr)[RevealerIndex.X][RevealerIndex.Y - Count] != ETileState::Obstacle)
					{
						if ((*GridDataPtr)[RevealerIndex.X][RevealerIndex.Y - Count] == ETileState::Visible)
						{
							bIsObstacleVisible = true;
						}
						else
						{
							bIsObstacleVisible = false;
						}
						break;
					}
				}
				if (bIsObstacleVisible)
				{
					EnemyAircraft->bIsExposedToEnemy = true;
				}
				else
				{
					EnemyAircraft->bIsExposedToEnemy = false;
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GridDataPtr is Nullptr"));
	}
}