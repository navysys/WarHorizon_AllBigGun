#include "FOW_Base.h"
#include "FOW_Obstacle.h"

AFOW_Base::AFOW_Base()
{
	PrimaryActorTick.bCanEverTick = false;


}

void AFOW_Base::BeginPlay()
{
	Super::BeginPlay();

	InitGrid();
}

void AFOW_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFOW_Base::AddBattleShipRevealer(AActor* NewFogActor, int Range, ETeamType Team)
{
	UFogRevealer* NewRevealer = NewObject<UFogRevealer>();
	NewRevealer->Revealer = NewFogActor;
	NewRevealer->SightRange = Range;
	BattleShipRevealers.Add(NewRevealer);
}

void AFOW_Base::AddAircraftRevealer(AActor* NewFogActor, int Range, ETeamType Team)
{
	UFogRevealer* NewRevealer = NewObject<UFogRevealer>();
	NewRevealer->Revealer = NewFogActor;
	NewRevealer->SightRange = Range;
	AircraftRevealers.Add(NewRevealer);
}

void AFOW_Base::RemoveBattleShipRevealer(AActor* NewFogActor, ETeamType Team)
{
	for (int i = 0; i < BattleShipRevealers.Num(); i++)
	{
		if (BattleShipRevealers[i]->Revealer == NewFogActor)
		{
			BattleShipRevealers.RemoveAt(i);
			break;
		}
	}
}

void AFOW_Base::RemoveAircraftRevealer(AActor* NewFogActor, ETeamType Team)
{
	for (int i = 0; i < AircraftRevealers.Num(); i++)
	{
		if (AircraftRevealers[i]->Revealer == NewFogActor)
		{
			AircraftRevealers.RemoveAt(i);
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

void AFOW_Base::InitGrid()
{
	GridDataPtr = new FGridData();

	// 그리드 위치 정보 가지는 스트럭쳐로 변경하고 디버깅할 수 있도록 하기
	int NumTilesPerSide = GridExtent / TileSize;
	for (int x = 0; x < NumTilesPerSide; x++)
	{
		TArray<ETileState> NewColumn;
		for (int y = 0; y < NumTilesPerSide; y++)
		{
			NewColumn.Add(ETileState::Invisible);
		}
		GridDataPtr->AddColumn(NewColumn);
	}
}

void AFOW_Base::ClearGrid()
{
	GridSize = GridExtent / TileSize;

	if (GridDataPtr != nullptr)
	{
		for (int x = 0; x < GridSize; x++)
		{
			for (int y = 0; y < GridSize; y++)
			{
				GridDataPtr->SetTileStateAt(x, y, ETileState::Invisible);
			}
		}
	}
}

FIntPoint AFOW_Base::GetGridIndexToWorldLocation(const FVector& WorldLocation)
{
	return FIntPoint::ZeroValue;
}

void AFOW_Base::ComputeFogOfWarVisibility(TArray<UFogRevealer*> BRevealers, TArray<UFogRevealer*> ARevealers)
{
	if (GridDataPtr != nullptr)
	{
		// 그리드 초기화
		ClearGrid();
		GridSize = GridExtent / TileSize;
		for (UFogRevealer* Revealer : BRevealers)
		{
			FVector RevealerLocation = Revealer->GetCurrentLocation();
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

					if (TileX >= 0 || TileY >= 0 || TileX <= GridSize || TileY <= GridSize)
					{
						if (!bIsObsticleInSightRange)	// 주변에 장해물이 없다면 주변 시야 원형으로 처리
						{
							GridDataPtr->SetTileStateAt(TileX, TileY, ETileState::Visible);
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
					GridDataPtr->SetTileStateAt(TileIndex.X, TileIndex.Y, ETileState::Obstacle);
					UE_LOG(LogTemp, Warning, TEXT("RaycastQueue Call"));
				}

			}
			// 장애물에 막히는 시야 처리
			for (UFogRevealer* Revealer : BFSRevealers)
			{
				FVector RevealerLocation = Revealer->GetCurrentLocation();
				FIntPoint RevealerIndex = GetGridIndexToWorldLocation(RevealerLocation);
				int SightRange = Revealer->SightRange;
				int RadiusTiles = FMath::CeilToInt(static_cast<float>(SightRange) / static_cast<float>(TileSize));
				ComputeFOV_BFS(*GridDataPtr, GridSize, GridSize, RevealerIndex, RadiusTiles);
			}
			LineTraceQueue.Empty();
			BFSRevealers.Empty();
		}

		// 항공기용 시야 처리
		for (UFogRevealer* Revealer : ARevealers)
		{
			FVector RevealerLocation = Revealer->GetCurrentLocation();
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

					if (!bIsObsticleInSightRange)
					{
						GridDataPtr->SetTileStateAt(TileX, TileY, ETileState::Visible);
					}
					else
					{
						LineTraceQueue.Add(FIntPoint(TileX, TileY));	// 중복처리 방지용 FSet
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
						GridDataPtr->SetTileStateAt(TileIndex.X, TileIndex.Y, ETileState::Obstacle);
						UE_LOG(LogTemp, Warning, TEXT("RaycastQueue Call"));
					}
					else
					{
						GridDataPtr->SetTileStateAt(TileIndex.X, TileIndex.Y, ETileState::Visible);
					}
				}
				LineTraceQueue.Empty();
			}
		}
	}
}

void AFOW_Base::ComputeFOV_BFS(FGridData& Grid, int GridSizeX, int GridSizeY, const FIntPoint& StartIndex, int ViewRadius)
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
	Grid.SetTileStateAt(StartIndex.X, StartIndex.Y, ETileState::Visible);

	const TArray<FIntPoint> Directions =
	{
		{ 1,  0}, {-1,  0},
		{ 0,  1}, { 0, -1}
	};

	int LoopCount = 0;
	while (!BFSQueue.IsEmpty())
	{
		LoopCount++;

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

			if (Grid[nx][ny] == ETileState::Obstacle)
			{
				continue;
			}

			Visited[nx][ny] = true;
			Grid.SetTileStateAt(nx, ny, ETileState::Visible);
			BFSQueue.Enqueue(FIntPoint(nx, ny));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("LoopCount : %i"), LoopCount);
}

