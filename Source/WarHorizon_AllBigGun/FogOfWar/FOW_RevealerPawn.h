// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enum/ETeamType.h"
#include "FOW_RevealerPawn.generated.h"

class AFOW_Manager;

UENUM()
enum class EFOWVisibility : uint8
{
	Visible = 0 UMETA(DisplayName = "Visible"),
	Hidden UMETA(DisplayName = "Hidden"),
};

UENUM()
enum class ERevealerType : uint8
{
	BattleShip = 0 UMETA(DisplayName = "BattleShip"),
	Aircraft UMETA(DisplayName = "Aircraft"),
};

UCLASS()
class WARHORIZON_ALLBIGGUN_API AFOW_RevealerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFOW_RevealerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_FOWVisibility();

	UFUNCTION()
	void RegisterToFogOfWarManager();

public:
	UPROPERTY(ReplicatedUsing = OnRep_FOWVisibility)
	EFOWVisibility FOWVisibility = EFOWVisibility::Hidden;

	UPROPERTY(BlueprintReadWrite, Category = "Fog Of War")
	int SightRange = 500;

	AFOW_Manager* FogManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog Of War")
	ETeamType TeamType = ETeamType::Invalid;
};
