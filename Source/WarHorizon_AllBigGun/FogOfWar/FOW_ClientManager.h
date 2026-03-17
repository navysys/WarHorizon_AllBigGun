// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarHorizon_AllBigGun/FogOfWar/FOW_Base.h"
#include "WarHorizon_AllBigGun/Enum/ETeamType.h"
#include "Rendering/Texture2DResource.h"
#include "FOW_ClientManager.generated.h"

class UPostProcessComponent;

USTRUCT()
struct FTexel2X2
{
	GENERATED_BODY()

	bool operator==(const FTexel2X2& Rhs) const
	{
		return T[0] == Rhs.T[0] && T[1] == Rhs.T[1] && T[2] == Rhs.T[2] && T[3] == Rhs.T[3];
	}
	uint8 T[4];
};
uint32 GetTypeHash(const FTexel2X2& Texel); // TMap을 사용하기 위한 커스텀 해시 함수

USTRUCT()
struct FTexel4X4
{
	GENERATED_BODY()

	uint8 T[4][4];
};

UCLASS()
class WARHORIZON_ALLBIGGUN_API AFOW_ClientManager : public AFOW_Base
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AFOW_ClientManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void UpdateGrid() override;
	void UpdateFogTexture();
	void UpdateTextuerBuffer();
	void GenerateUpscaleMap();
	/** (X,Y), (X+1,Y), (X,Y+1), (X+1,Y+1) 텍셀을 가져옵니다. */
	FTexel2X2 GetTexel2X2(int X, int Y);
public:
	UPROPERTY()
	UPostProcessComponent* FogPostProcessVolume;


public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	UMaterialInterface* FogMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	UMaterialInterface* FogMiniMapMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Fog Of War")
	UMaterialInstanceDynamic* FogMaterialInstance;

	UPROPERTY(BlueprintReadOnly, Category = "Fog Of War")
	UMaterialInstanceDynamic* FogMiniMapMaterialInstance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	UTexture2D* FogTexture;

	TArray<uint8> TextureBuffer;
	FUpdateTextureRegion2D UpscaleUpdateRegion;

	/** 2X2 텍셀을 4X4 텍셀로 어떻게 맵핑할지 저장해 놓은 템플릿입니다. */
	UPROPERTY()
	TMap<FTexel2X2, FTexel4X4> UpscaleMap;

	float TextureRatio = 4;

	UPROPERTY(BlueprintReadWrite, Category = "Fog Of War")
	FVector FogColor = FVector(5, 15, 25);

	UPROPERTY(BlueprintReadWrite, Category = "Fog Of War")
	int32 TextureSize = 2000;

	UPROPERTY(BlueprintReadWrite, Category = "Fog Of War")
	float FogBlurStrength = 1.0f;
};
