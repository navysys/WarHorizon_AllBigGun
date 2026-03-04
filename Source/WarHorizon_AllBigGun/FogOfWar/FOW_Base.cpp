#include "FOW_Base.h"
#include "FOW_Obstacle.h"
#include "FogOfWar/FOW_RevealerPawn.h"

AFOW_Base::AFOW_Base()
{
	PrimaryActorTick.bCanEverTick = false;


}

void AFOW_Base::BeginPlay()
{
	Super::BeginPlay();

	GridSize = GridExtent / TileSize;
	GridDataArray.SetNumZeroed(GridSize * GridSize);
	
}

void AFOW_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFOW_Base::AddRevealer(AFOW_RevealerPawn* RevealerPawn)
{
	if (IsValid(RevealerPawn))
	{
		if (RevealerPawn->RevealerType == ERevealerType::BattleShip)
		{
			BattleShipRevealers.Add(RevealerPawn);
		}
		else if (RevealerPawn->RevealerType == ERevealerType::Aircraft)
		{
			AircraftRevealers.Add(RevealerPawn);
		}
	}
	
}

void AFOW_Base::RemoveRevealer(AFOW_RevealerPawn* RevealerPawn)
{
	for (int i = 0; i < BattleShipRevealers.Num(); i++)
	{
		if (BattleShipRevealers[i] == RevealerPawn)
		{
			BattleShipRevealers.RemoveAt(i);
			break;
		}
	}
}

void AFOW_Base::RegisterFOWObstacle(AFOW_Obstacle* Obstacle)
{
	AllFogObstacle.Add(Obstacle);
}

void AFOW_Base::RemoveFOWObstacle(AFOW_Obstacle* Obstacle)
{
	AllFogObstacle.Remove(Obstacle);
}

FIntPoint AFOW_Base::GetGridIndexToWorldLocation(const FVector& WorldLocation)
{
	int32 X = FMath::FloorToInt((WorldLocation.X + GridExtent / 2) / TileSize);
	int32 Y = FMath::FloorToInt((WorldLocation.Y + GridExtent / 2) / TileSize);

	X = FMath::Clamp(X, 0, GridSize - 1);
	Y = FMath::Clamp(Y, 0, GridSize - 1);

	return FIntPoint(X, Y);
}

void AFOW_Base::ComputeFogOfWarVisibility(TArray<AFOW_RevealerPawn*> BRevealers, TArray<AFOW_RevealerPawn*> ARevealers)
{
	// 그리드 초기화
	//ClearGrid();

	for (AFOW_RevealerPawn* Revealer : BRevealers)
	{
		FVector RevealerLocation = Revealer->GetActorLocation();
		int SightRange = Revealer->SightRange;
		bool bIsObsticleInSightRange = false;
		// Revealer 의 시야 범위 내에 장애물이 있는지 확인
		for (AActor* Obstacle : AllFogObstacle)
		{
			if (IsValid(Obstacle))
			{
				FVector Interval = Obstacle->GetActorLocation() - RevealerLocation;
				float Distance = Interval.SizeSquared();	// 제곱근 계산 없이 비교
				if (Distance < FMath::Square(SightRange))
				{
					bIsObsticleInSightRange = true;
					BFSRevealers.Add(Revealer);	// BFS 처리용 큐에 추가

					break;
				}
			}
		}

		FIntPoint RevealerIndex = GetGridIndexToWorldLocation(RevealerLocation);
		int RadiusTiles = FMath::CeilToInt(static_cast<float>(SightRange) / static_cast<float>(TileSize));

		for (int dx = -RadiusTiles; dx <= RadiusTiles; dx++)
		{
			for (int dy = -RadiusTiles; dy <= RadiusTiles; dy++)
			{
				// 원 형태 체크: (dx² + dy² <= r²)
				if (dx * dx + dy * dy > RadiusTiles * RadiusTiles)
					continue;

				int TileX = RevealerIndex.X + dx;
				int TileY = RevealerIndex.Y + dy;

				if (TileX >= 0 && TileY >= 0 && TileX < GridSize && TileY < GridSize)
				{
					if (!bIsObsticleInSightRange)	// 주변에 장해물이 없다면 주변 시야 원형으로 처리
					{
						GridDataArray[TileY * GridSize + TileX] = 255;
					}
					else    // 장애물이 있다면 레이캐스트 큐에 추가
					{
						LineTraceQueue.Add(FIntPoint(TileX, TileY));	// 중복처리 방지용 FSet
					}
				}
			}
		}
	}

	// 레이캐스트 큐 처리
	if (LineTraceQueue.Num() > 0)
	{
		for (const FIntPoint& TileIndex : LineTraceQueue)
		{
			FVector TileWorldPos = FVector((TileIndex.X - GridSize * 0.5f + 0.5f) * TileSize, (TileIndex.Y - GridSize * 0.5f + 0.5f) * TileSize, 0.0f);

			FVector TraceStart = TileWorldPos + FVector(0.0f, 0.0f, 500.f);
			FVector TraceEnd = TileWorldPos - FVector(0.0f, 0.0f, 500.f);

			FHitResult OutHit;
			GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECollisionChannel::ECC_GameTraceChannel14);
			if (OutHit.bBlockingHit)
			{
				ObstacleCoordinates.Add(TileIndex);
			}

		}
		// 장애물에 막히는 시야 처리
		for (AFOW_RevealerPawn* Revealer : BFSRevealers)
		{
			FVector RevealerLocation = Revealer->GetActorLocation();
			FIntPoint RevealerIndex = GetGridIndexToWorldLocation(RevealerLocation);
			int SightRange = Revealer->SightRange;
			int RadiusTiles = FMath::CeilToInt(static_cast<float>(SightRange) / static_cast<float>(TileSize));
			ComputeFOV_BFS(GridDataArray, GridSize, GridSize, RevealerIndex, RadiusTiles);
		}
		LineTraceQueue.Empty();
		BFSRevealers.Empty();
	}

	// 항공기용 시야 처리
	for (AFOW_RevealerPawn* Revealer : ARevealers)
	{
		FVector RevealerLocation = Revealer->GetActorLocation();
		int SightRange = Revealer->SightRange;
		bool bIsObsticleInSightRange = false;
		// Revealer 의 시야 범위 내에 장애물이 있는지 확인
		for (AActor* Obstacle : AllFogObstacle)
		{
			if (IsValid(Obstacle))
			{
				FVector Interval = Obstacle->GetActorLocation() - RevealerLocation;
				float Distance = Interval.SizeSquared();	// 제곱근 계산 없이 비교
				if (Distance < FMath::Square(SightRange))
				{
					bIsObsticleInSightRange = true;

					break;
				}
			}
		}

		// 장애물이 없다면 원형 시야 처리
		FIntPoint CenterTile = GetGridIndexToWorldLocation(RevealerLocation);
		int RadiusTiles = FMath::CeilToInt(static_cast<float>(SightRange) / static_cast<float>(TileSize));
		for (int dx = -RadiusTiles; dx <= RadiusTiles; dx++)
		{
			for (int dy = -RadiusTiles; dy <= RadiusTiles; dy++)
			{
				// 원 형태 체크: (dx² + dy² <= r²)
				if (dx * dx + dy * dy > RadiusTiles * RadiusTiles)
					continue;

				int TileX = CenterTile.X + dx;
				int TileY = CenterTile.Y + dy;

				if (TileX >= 0 && TileY >= 0 && TileX < GridSize && TileY < GridSize)
				{
					if (!bIsObsticleInSightRange)
					{
						GridDataArray[TileY * GridSize + TileX] = 255;
					}
					else
					{
						LineTraceQueue.Add(FIntPoint(TileX, TileY));	// 중복처리 방지용 FSet
					}
				}

			}
		}

		// 레이캐스트 큐 처리
		if (LineTraceQueue.Num() > 0)
		{
			for (const FIntPoint& TileIndex : LineTraceQueue)
			{
				FVector TileWorldPos = FVector((TileIndex.X - GridSize * 0.5f + 0.5f) * TileSize, (TileIndex.Y - GridSize * 0.5f + 0.5f) * TileSize, 0.0f);

				FVector TraceStart = TileWorldPos + FVector(0.0f, 0.0f, 500.f);
				FVector TraceEnd = TileWorldPos - FVector(0.0f, 0.0f, 500.f);

				FHitResult OutHit;
				GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECollisionChannel::ECC_GameTraceChannel14);
				if (OutHit.bBlockingHit)
				{
					ObstacleCoordinates.Add(TileIndex);
					UE_LOG(LogTemp, Warning, TEXT("RaycastQueue Call"));
				}
				else
				{
					GridDataArray[TileIndex.Y * GridSize + TileIndex.X] = 255;
				}
			}
			LineTraceQueue.Empty();
		}
	}
}

void AFOW_Base::ComputeFOV_BFS(TArray<uint8>& Grid, int GridSizeX, int GridSizeY, const FIntPoint& StartIndex, int ViewRadius)
{
	if (StartIndex.X < 0 || StartIndex.Y < 0 || StartIndex.X >= GridSizeX || StartIndex.Y >= GridSizeY)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeFOV_BFS: Invalid StartIndex (%d, %d)"), StartIndex.X, StartIndex.Y);
		return;
	}

	TQueue<FIntPoint> BFSQueue;
	TArray<TArray<bool>> Visited;
	Visited.SetNum(GridSizeX);
	for (int i = 0; i < GridSizeX; i++)
	{
		Visited[i].SetNum(GridSizeY);
		for (int j = 0; j < GridSizeY; j++)
		{
			Visited[i][j] = false;
		}
	}

	BFSQueue.Enqueue(StartIndex);
	Visited[StartIndex.X][StartIndex.Y] = true;
	Grid[StartIndex.Y * GridSize + StartIndex.X] = 255; // 시작점은 항상 보이는 상태로 설정

	while (!BFSQueue.IsEmpty())
	{
		FIntPoint CurrentIndex;
		BFSQueue.Dequeue(CurrentIndex);

		int32 dx = CurrentIndex.X - StartIndex.X;
		int32 dy = CurrentIndex.Y - StartIndex.Y;

		if (dx * dx + dy * dy > ViewRadius * ViewRadius)
		{
			continue;
		}

		for (const FIntPoint& Dir : Directions)
		{
			int nx = CurrentIndex.X + Dir.X;
			int ny = CurrentIndex.Y + Dir.Y;

			if (nx < 0 || ny < 0 || nx >= GridSizeX || ny >= GridSizeY)
			{
				continue;
			}

			if (Visited[nx][ny])
			{
				continue;
			}

			if (ObstacleCoordinates.Find(FIntPoint(nx, ny)) != INDEX_NONE)
			{
				continue;
			}

			Visited[nx][ny] = true;
			Grid[ny * GridSize + nx] = 255;
			BFSQueue.Enqueue(FIntPoint(nx, ny));
		}
	}
}

void AFOW_Base::UpdateSideGrid()
{
	for (int x = 0; x < GridSize; x++)
	{
		for(int y = 0; y < GridSize; y++)
		{
			if (GridDataArray[y * GridSize + x] == 255)
			{
				for (const FIntPoint& Dir : Directions)
				{
					int nx = x + Dir.X;
					int ny = y + Dir.Y;
					if (nx < 0 || ny < 0 || nx >= GridSize || ny >= GridSize)
					{
						continue;
					}
					if (GridDataArray[ny * GridSize + nx] == 0)
					{
						GridDataArray[ny * GridSize + nx] = 125;
						break;
					}
				}
			}
		}
	}
}

