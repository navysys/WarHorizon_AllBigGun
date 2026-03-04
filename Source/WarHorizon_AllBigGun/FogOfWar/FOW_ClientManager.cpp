// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FOW_ClientManager.h"
#include "Components/PostProcessComponent.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"



// Sets default values
AFOW_ClientManager::AFOW_ClientManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	FogPostProcessVolume = CreateDefaultSubobject<UPostProcessComponent>(TEXT("FogPostProcessVolume"));
	FogPostProcessVolume->bEnabled = true;
	FogPostProcessVolume->bUnbound = true;
}

// Called when the game starts or when spawned
void AFOW_ClientManager::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(FOVTimerHandle, this, &AFOW_ClientManager::UpdateGrid, RefreshRate, true);

	FogMaterialInstance = UMaterialInstanceDynamic::Create(FogMaterial, this);

	FogPostProcessVolume->Settings.WeightedBlendables.Array.Empty();
	FogPostProcessVolume->AddOrUpdateBlendable(FogMaterialInstance);

	//GridSize = GridExtent / TileSize;
	//FogMaterialInstance->SetScalarParameterValue(FName("GridSize"), GridSize);
	//FogGridTexture = UTexture2D::CreateTransient(GridSize, GridSize, PF_R8);

	//FogGridTexture->CompressionSettings = TC_Masks;
	//FogGridTexture->Filter = TF_Bilinear;
	//FogGridTexture->SRGB = false;
	//FogGridTexture->AddToRoot();
	//FogGridTexture->UpdateResource();

	// 버퍼 초기화
	TextureBuffer.SetNumZeroed(TextureSize * TextureSize);
	FogMaterialInstance->SetScalarParameterValue(FName("TextureSize"), TextureSize);

	// 텍스쳐 초기화
	FogGridTexture = UTexture2D::CreateTransient(TextureSize, TextureSize, PF_G8);
	FogGridTexture->CompressionSettings = TC_Grayscale;
	FogGridTexture->SRGB = false;
	FogGridTexture->Filter = TF_Nearest;
	FogGridTexture->AddressX = TA_Clamp;
	FogGridTexture->AddressY = TA_Clamp;
	FogGridTexture->UpdateResource();

	// 전체 영역 설정
	FullRegion = FUpdateTextureRegion2D(
		0, 0,      // DestX, DestY
		0, 0,      // SrcX, SrcY
		TextureSize,
		TextureSize
	);
}

// Called every frame
void AFOW_ClientManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update Fog Texture
	//FTexture2DMipMap& Mip = FogGridTexture->GetPlatformData()->Mips[0];
	//void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);

	//uint8* Pixels = static_cast<uint8*>(Data);
	//for (int y = 0; y < GridSize; y++)
	//{
	//	for (int x = 0; x < GridSize; x++)
	//	{
	//		bool bVisible;
	//		if ((*GridDataPtr)[x][y] == ETileState::Visible)
	//		{
	//			bVisible = true;
	//		}
	//		else
	//		{
	//			bVisible = false;
	//		}
	//		Pixels[y * GridSize + x] = bVisible ? 255 : 0;
	//	}
	//}

	//Mip.BulkData.Unlock();
	//FogGridTexture->UpdateResource();

	FogMaterialInstance->SetTextureParameterValue(FName("FogGridTexture"), FogGridTexture);


	// 화면 코너 좌표 월드 좌표 변환
	//FVector2D ViewportSize;
	//GEngine->GameViewport->GetViewportSize(ViewportSize);

	//TArray<FVector2D> ScreenCorners =
	//{
	//	FVector2D(0.f, 0.f),
	//	FVector2D(ViewportSize.X, 0.f),
	//	FVector2D(0.f, ViewportSize.Y),
	//	FVector2D(ViewportSize.X, ViewportSize.Y)
	//};

	//APlayerController* PC = GetWorld()->GetFirstPlayerController();
	//APlayerCameraManager* PCM = PC->PlayerCameraManager;
	//float CameraPitch = PCM->GetCameraRotation().Pitch;
	//float CameraPitchRad = FMath::DegreesToRadians(CameraPitch);
	//FogMaterialInstance->SetScalarParameterValue(FName("CameraPitch"), CameraPitchRad);

	//TArray<FVector> WorldCorners;
	//WorldCorners.Empty();

	//for (const FVector2D& ScreenPos : ScreenCorners)
	//{
	//	FVector WorldOrigin;
	//	FVector WorldDirection;

	//	if (PC->DeprojectScreenPositionToWorld(ScreenPos.X, ScreenPos.Y, WorldOrigin, WorldDirection))
	//	{
	//		//UE_LOG(LogTemp, Warning, TEXT("WorldOrigin : %s"), *WorldOrigin.ToString());
	//		//UE_LOG(LogTemp, Warning, TEXT("WorldDirection : %s"), *WorldDirection.ToString());

	//		FHitResult Hit;
	//		bool bHit = GetWorld()->LineTraceSingleByChannel(
	//			Hit,
	//			WorldOrigin,
	//			WorldOrigin + WorldDirection * 100000.f,
	//			ECC_Visibility
	//		);

	//		if (bHit)
	//		{
	//			FVector GroundPos = Hit.ImpactPoint;
	//			WorldCorners.Add(GroundPos);
	//			//UE_LOG(LogTemp, Warning, TEXT("GroundPos : %s"), *GroundPos.ToString());
	//		}
	//	}
	//}

	//FVector WorldCenter = FVector::ZeroVector;
	//if (WorldCorners.Num() > 3)
	//{
	//	float Width = FVector::Distance(WorldCorners[0], WorldCorners[1]);
	//	float Height = FVector::Distance(WorldCorners[0], WorldCorners[2]);
	//	float WidthRatio = Width / GridExtent;
	//	float HeightRatio = Height / GridExtent;

	//	for (const FVector& Corner : WorldCorners)
	//	{
	//		WorldCenter += Corner;
	//	}
	//	WorldCenter /= WorldCorners.Num();

	//	FLinearColor WorldRatio = FLinearColor(WidthRatio, HeightRatio, WorldCenter.X / GridExtent, WorldCenter.Y / GridExtent);
	//	FogMaterialInstance->SetVectorParameterValue(FName("WorldRatio"), WorldRatio);
	//}
}

void AFOW_ClientManager::UpdateGrid()
{
	ComputeFogOfWarVisibility(BattleShipRevealers, AircraftRevealers);
	UpdateSideGrid();
	UpdateFogTexture();
}

void AFOW_ClientManager::UpdateFogTexture()
{
	for (int32 X = 0; X < TextureSize; ++X)
	{
		for (int32 Y = 0; Y < TextureSize; ++Y)
		{
			int32 GridX = FMath::FloorToInt(static_cast<float>(X) / TextureRatio);
			int32 GridY = FMath::FloorToInt(static_cast<float>(Y) / TextureRatio);
			
			if(GridDataArray[GridY * GridSize + GridX] == 255)
			{
				TextureBuffer[Y * TextureSize + X] = 255;
			}
			else if(GridDataArray[GridY * GridSize + GridX] == 125)
			{
				// 현재 테스트용으로 Side 타일도 Visible 처럼 보이도록 설정
				TextureBuffer[Y * TextureSize + X] = 125;
			}
			else
			{
				TextureBuffer[Y * TextureSize + X] = 0;
			}
		}
	}


	if (!FogGridTexture) return;

	FogGridTexture->UpdateTextureRegions(
		0,                          // MipIndex
		1,                          // NumRegions
		&FullRegion,                // Regions
		2000,                   // SrcPitch (한 줄 byte 수)
		1,                          // BytesPerPixel (PF_R8)
		TextureBuffer.GetData()     // SrcData
	);
}
