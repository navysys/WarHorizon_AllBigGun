#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WHGameMode.generated.h"

class AWHBattleShipBase;
class AFOW_Manager;

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AWHGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	AFOW_Manager* GetFogOfWarManager() { return FogOfWarManager; }

public:
	UPROPERTY(BlueprintReadOnly, Category = "Fog Of War")
	AFOW_Manager* FogOfWarManager;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TSubclassOf<AFOW_Manager> FogOfWarManagerClass;

	UPROPERTY(EditAnywhere, Category = "WHGameMode | Red Team")
	TArray<APlayerController*> RedTeamControllers;
	UPROPERTY(EditAnywhere, Category = "WHGameMode | Blue Team")
	TArray<APlayerController*> BlueTeamControllers;

	UPROPERTY(EditAnywhere, Category = "WHGameMode | Red Team")
	TArray<TSubclassOf<AWHBattleShipBase>> RedTeamBattleShipClasses;
	UPROPERTY(EditAnywhere, Category = "WHGameMode | Blue Team")
	TArray<TSubclassOf<AWHBattleShipBase>> BlueTeamBattleShipClasses;

	UPROPERTY(BlueprintReadOnly, Category = "WHGameMode | Red Team")
	TArray<AWHBattleShipBase*> RedTeamBattleShips;
	UPROPERTY(BlueprintReadOnly, Category = "WHGameMode | Blue Team")
	TArray<AWHBattleShipBase*> BlueTeamBattleShips;
};
