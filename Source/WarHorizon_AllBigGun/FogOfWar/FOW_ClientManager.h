// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogOfWar/FOW_Base.h"
#include "Enum/ETeamType.h"
#include "FOW_ClientManager.generated.h"

class UPostProcessComponent;
class UMaterialInstanceDynamic;


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

public:
	UPROPERTY()
	UPostProcessComponent* FogPostProcessVolume;


public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	UMaterialInterface* FogMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Fog Of War")
	UMaterialInstanceDynamic* FogMaterialInstance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	UTexture2D* FogGridTexture;

	UPROPERTY(BlueprintReadWrite, Category = "Fog Of War")
	FVector FogColor = FVector(5, 15, 25);

	UPROPERTY(BlueprintReadWrite, Category = "Fog Of War")
	float FogBlurStrength = 1.0f;
};
