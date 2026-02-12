// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FOW_ClientManager.h"
#include "Components/PostProcessComponent.h"

// Sets default values
AFOW_ClientManager::AFOW_ClientManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FogPostProcessVolume = CreateDefaultSubobject<UPostProcessComponent>(TEXT("FogPostProcessVolume"));
	FogPostProcessVolume->bEnabled = true;
	FogPostProcessVolume->bUnbound = true;
}

// Called when the game starts or when spawned
void AFOW_ClientManager::BeginPlay()
{
	Super::BeginPlay();
	
	//InitGrid();
	UE_LOG(LogTemp, Warning, TEXT("APlayerFogActor : BeginPlay"));
	GetWorld()->GetTimerManager().SetTimer(FOVTimerHandle, this, &AFOW_ClientManager::UpdateGrid, RefreshRate, true);

	FogMaterialInstance = UMaterialInstanceDynamic::Create(FogMaterial, this);
	FogMaterialInstance->SetScalarParameterValue(FName("GridExtent"), GridExtent);

	FogPostProcessVolume->Settings.WeightedBlendables.Array.Empty();
	FogPostProcessVolume->AddOrUpdateBlendable(FogMaterialInstance);

	GridSize = GridExtent / TileSize;
	FogGridTexture = UTexture2D::CreateTransient(GridSize, GridSize, PF_R8);

	FogGridTexture->CompressionSettings = TC_Grayscale;
	FogGridTexture->Filter = TF_Nearest;
	FogGridTexture->SRGB = false;
	FogGridTexture->AddToRoot();
	FogGridTexture->UpdateResource();
}

// Called every frame
void AFOW_ClientManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update Fog Texture
	FTexture2DMipMap& Mip = FogGridTexture->GetPlatformData()->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);

	uint8* Pixels = static_cast<uint8*>(Data);
	for (int y = 0; y < GridSize; y++)
	{
		for (int x = 0; x < GridSize; x++)
		{
			bool bVisible;
			if ((*GridDataPtr)[x][y] == ETileState::Visible)
			{
				bVisible = true;
			}
			else
			{
				bVisible = false;
			}
			Pixels[y * GridSize + x] = bVisible ? 255 : 0;
		}
	}

	Mip.BulkData.Unlock();
	FogGridTexture->UpdateResource();

	FogMaterialInstance->SetTextureParameterValue(FName("FogGridTexture"), FogGridTexture);


	// 화면 코너 좌표 월드 좌표 변환
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	TArray<FVector2D> ScreenCorners =
	{
		FVector2D(0.f, 0.f),
		FVector2D(ViewportSize.X, 0.f),
		FVector2D(0.f, ViewportSize.Y),
		FVector2D(ViewportSize.X, ViewportSize.Y)
	};

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	APlayerCameraManager* PCM = PC->PlayerCameraManager;
	float CameraPitch = PCM->GetCameraRotation().Pitch;
	float CameraPitchRad = FMath::DegreesToRadians(CameraPitch);
	FogMaterialInstance->SetScalarParameterValue(FName("CameraPitch"), CameraPitchRad);

	TArray<FVector> WorldCorners;
	WorldCorners.Empty();

	for (const FVector2D& ScreenPos : ScreenCorners)
	{
		FVector WorldOrigin;
		FVector WorldDirection;

		if (PC->DeprojectScreenPositionToWorld(ScreenPos.X, ScreenPos.Y, WorldOrigin, WorldDirection))
		{
			//UE_LOG(LogTemp, Warning, TEXT("WorldOrigin : %s"), *WorldOrigin.ToString());
			//UE_LOG(LogTemp, Warning, TEXT("WorldDirection : %s"), *WorldDirection.ToString());

			FHitResult Hit;
			bool bHit = GetWorld()->LineTraceSingleByChannel(
				Hit,
				WorldOrigin,
				WorldOrigin + WorldDirection * 100000.f,
				ECC_Visibility
			);

			if (bHit)
			{
				FVector GroundPos = Hit.ImpactPoint;
				WorldCorners.Add(GroundPos);
				//UE_LOG(LogTemp, Warning, TEXT("GroundPos : %s"), *GroundPos.ToString());
			}
		}
	}
	if (WorldCorners.Num() > 3)
	{
		float Width = FVector::Distance(WorldCorners[0], WorldCorners[1]);
		float Height = FVector::Distance(WorldCorners[0], WorldCorners[2]);
		float WidthRatio = Width / GridExtent;
		float HeightRatio = Height / GridExtent;
		FogMaterialInstance->SetScalarParameterValue(FName("WidthRatio"), WidthRatio);
		FogMaterialInstance->SetScalarParameterValue(FName("HeightRatio"), HeightRatio);
	}
}

void AFOW_ClientManager::UpdateGrid()
{
	ComputeFogOfWarVisibility(BattleShipRevealers, AircraftRevealers);
}
