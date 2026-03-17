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

	GenerateUpscaleMap();
}

// Called when the game starts or when spawned
void AFOW_ClientManager::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(FOVTimerHandle, this, &AFOW_ClientManager::UpdateGrid, RefreshRate, true);

	FogMaterialInstance = UMaterialInstanceDynamic::Create(FogMaterial, this);
	FogMiniMapMaterialInstance = UMaterialInstanceDynamic::Create(FogMiniMapMaterial, this);

	FogPostProcessVolume->Settings.WeightedBlendables.Array.Empty();
	FogPostProcessVolume->AddOrUpdateBlendable(FogMaterialInstance);

	// 버퍼 초기화
	TextureBuffer.SetNumZeroed(TextureSize * TextureSize);

	// 텍스쳐 초기화
	FogTexture = UTexture2D::CreateTransient(TextureSize, TextureSize, PF_G8);
	FogTexture->CompressionSettings = TC_Grayscale;
	FogTexture->SRGB = false;
	FogTexture->Filter = TF_Nearest;
	FogTexture->AddressX = TA_Clamp;
	FogTexture->AddressY = TA_Clamp;
	FogTexture->UpdateResource();
	FogMaterialInstance->SetTextureParameterValue(FName("FogTexture"), FogTexture);
	FogMiniMapMaterialInstance->SetTextureParameterValue(FName("FogTexture"), FogTexture);

	// 전체 영역 설정
	UpscaleUpdateRegion = FUpdateTextureRegion2D(
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

}

void AFOW_ClientManager::UpdateGrid()
{
	ComputeFogOfWarVisibility(BattleShipRevealers, AircraftRevealers);
	UpdateTextuerBuffer();
	UpdateFogTexture();
}

void AFOW_ClientManager::UpdateFogTexture()
{
	if (!FogTexture) return;

	FogTexture->UpdateTextureRegions(
		0,                          // MipIndex
		1,                          // NumRegions
		&UpscaleUpdateRegion,                // Regions
		2000,                   // SrcPitch (한 줄 byte 수)
		1,                          // BytesPerPixel (PF_R8)
		TextureBuffer.GetData()     // SrcData
	);

	
}

void AFOW_ClientManager::UpdateTextuerBuffer()
{
	FTexel2X2 Texel2X2;
	FTexel4X4* Texel4X4;

	for (int i = 0; i < GridSize; ++i)
	{
		for (int j = 0; j < GridSize; ++j)
		{
			Texel2X2 = GetTexel2X2(i, j);

			Texel4X4 = UpscaleMap.Find(Texel2X2);
			if (Texel4X4 == nullptr)
			{
				continue;
			}

			for (int Row = 0; Row < 4; ++Row)
			{
				int Index = (Row + j * 4) * TextureSize + i * 4;
				FMemory::Memcpy(&TextureBuffer[Index], &Texel4X4->T[Row], sizeof(uint8) * 4);
			}
		}
	}
}

void AFOW_ClientManager::GenerateUpscaleMap()
{
	UpscaleMap.Reserve(16);

	UpscaleMap.Emplace(FTexel2X2{ 0, 0, 0, 0 }, FTexel4X4{ { { 0, 0, 0, 0 },
															 { 0, 0, 0, 0 },
															 { 0, 0, 0, 0 },
															 { 0, 0, 0, 0 } } });

	UpscaleMap.Emplace(FTexel2X2{ 0xFF, 0, 0, 0 }, FTexel4X4{ { { 0xFF, 0x80, 0,    0	 },
																{ 0x80, 0,	  0,    0	 },
																{ 0,	0,	  0,    0	 },
																{ 0,	0,	  0,    0	 } } });

	UpscaleMap.Emplace(FTexel2X2{ 0, 0xFF, 0, 0 }, FTexel4X4{ { { 0,    0,    0x80, 0xFF },
																{ 0,    0,    0,	0x80 },
																{ 0,    0,    0,	0	 },
																{ 0,    0,    0,	0	 } } });

	UpscaleMap.Emplace(FTexel2X2{ 0, 0, 0xFF, 0 }, FTexel4X4{ { { 0,	0,	  0,    0    },
																{ 0,	0,	  0,    0    },
																{ 0x80, 0,	  0,    0    },
																{ 0xFF, 0x80, 0,    0    } } });

	UpscaleMap.Emplace(FTexel2X2{ 0, 0, 0, 0xFF }, FTexel4X4{ { { 0,    0,    0,    0    },
																{ 0,    0,    0,    0    },
																{ 0,    0,    0,    0x80 },
																{ 0,    0,    0x80, 0xFF } } });

	UpscaleMap.Emplace(FTexel2X2{ 0xFF, 0xFF, 0, 0 }, FTexel4X4{ { { 0xFF, 0xFF, 0xFF, 0xFF },
																   { 0xFF, 0xFF, 0xFF, 0xFF },
																   { 0,    0,	 0,	   0	},
																   { 0,    0,	 0,	   0	} } });

	UpscaleMap.Emplace(FTexel2X2{ 0xFF, 0, 0xFF, 0 }, FTexel4X4{ { { 0xFF, 0xFF, 0, 0 },
																   { 0xFF, 0xFF, 0, 0 },
																   { 0xFF, 0xFF, 0, 0 },
																   { 0xFF, 0xFF, 0, 0 } } });

	UpscaleMap.Emplace(FTexel2X2{ 0xFF, 0, 0, 0xFF }, FTexel4X4{ { { 0xFF, 0x80, 0,	   0    },
																   { 0x80, 0,	 0,	   0    },
																   { 0,	   0,	 0,	   0x80 },
																   { 0,	   0,	 0x80, 0xFF } } });

	UpscaleMap.Emplace(FTexel2X2{ 0, 0xFF, 0xFF, 0 }, FTexel4X4{ { { 0,	   0,	 0x80, 0xFF },
																   { 0,	   0,	 0,	   0x80 },
																   { 0x80, 0,	 0,	   0    },
																   { 0xFF, 0x80, 0,	   0    } } });

	UpscaleMap.Emplace(FTexel2X2{ 0, 0xFF, 0, 0xFF }, FTexel4X4{ { { 0,	   0,	 0xFF, 0xFF },
																   { 0,	   0,	 0xFF, 0xFF },
																   { 0,	   0,	 0xFF, 0xFF },
																   { 0,	   0,	 0xFF, 0xFF } } });

	UpscaleMap.Emplace(FTexel2X2{ 0, 0, 0xFF, 0xFF }, FTexel4X4{ { { 0,	   0,	 0,	   0    },
																   { 0,	   0,	 0,	   0    },
																   { 0xFF, 0xFF, 0xFF, 0xFF },
																   { 0xFF, 0xFF, 0xFF, 0xFF } } });

	UpscaleMap.Emplace(FTexel2X2{ 0xFF, 0xFF, 0xFF, 0 }, FTexel4X4{ { { 0xFF, 0xFF, 0xFF, 0xFF },
																	  { 0xFF, 0xFF, 0xFF, 0xFF },
																	  { 0xFF, 0xFF, 0xFF, 0x80 },
																	  { 0xFF, 0xFF, 0x80, 0	   } } });

	UpscaleMap.Emplace(FTexel2X2{ 0xFF, 0xFF, 0, 0xFF }, FTexel4X4{ { { 0xFF, 0xFF, 0xFF, 0xFF },
																	  { 0xFF, 0xFF, 0xFF, 0xFF },
																	  { 0x80, 0xFF, 0xFF, 0xFF },
																	  { 0,	  0x80, 0xFF, 0xFF } } });

	UpscaleMap.Emplace(FTexel2X2{ 0xFF, 0, 0xFF, 0xFF }, FTexel4X4{ { { 0xFF, 0xFF, 0x80, 0	   },
																	  { 0xFF, 0xFF, 0xFF, 0x80 },
																	  { 0xFF, 0xFF, 0xFF, 0xFF },
																	  { 0xFF, 0xFF, 0xFF, 0xFF } } });

	UpscaleMap.Emplace(FTexel2X2{ 0, 0xFF, 0xFF, 0xFF }, FTexel4X4{ { { 0,	  0x80,	0xFF, 0xFF },
																	  { 0x80, 0xFF, 0xFF, 0xFF },
																	  { 0xFF, 0xFF, 0xFF, 0xFF },
																	  { 0xFF, 0xFF, 0xFF, 0xFF } } });

	UpscaleMap.Emplace(FTexel2X2{ 0xFF, 0xFF, 0xFF, 0xFF }, FTexel4X4{ { { 0xFF, 0xFF, 0xFF, 0xFF },
																		 { 0xFF, 0xFF, 0xFF, 0xFF },
																		 { 0xFF, 0xFF, 0xFF, 0xFF },
																		 { 0xFF, 0xFF, 0xFF, 0xFF } } });
}

FTexel2X2 AFOW_ClientManager::GetTexel2X2(int X, int Y)
{
	int NextX = FMath::Min(X + 1, GridSize - 1);
	int NextY = FMath::Min(Y + 1, GridSize - 1);

	FTexel2X2 Texel;
	Texel.T[0] = GridDataArray[Y * GridSize + X];
	Texel.T[1] = GridDataArray[Y * GridSize + NextX];
	Texel.T[2] = GridDataArray[NextY * GridSize + X];
	Texel.T[3] = GridDataArray[NextY * GridSize + NextX];

	return Texel;
}

uint32 GetTypeHash(const FTexel2X2& Texel)
{
	return Texel.T[0] * 1000 + Texel.T[1] * 100 + Texel.T[2] * 10 + Texel.T[3];
}
