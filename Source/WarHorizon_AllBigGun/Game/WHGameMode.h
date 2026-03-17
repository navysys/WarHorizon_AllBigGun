#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WHGameMode.generated.h"

class AWHBattleShipBase;
class AFOW_Manager;

USTRUCT()
struct FPlayerInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	APlayerController* PlayerController = nullptr;
	UPROPERTY(EditAnywhere)
	uint16 BattleShipID = 11000;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWHBattleShipBase> BattleShipClass = nullptr;
	UPROPERTY(EditAnywhere)
	AWHBattleShipBase* BattleShip = nullptr;
};

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AWHGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;
	//virtual void PostLogin(APlayerController* NewPlayer) override;

	AFOW_Manager* GetFogOfWarManager() { return FogOfWarManager; }

	void SetPlayerInfoBattleShipClass(TMap<FString, FPlayerInfo>& PlayerInfo);
	void SpawnBattleShipForPlayer(APlayerController* PlayerController, uint16 BattleShipID);
public:
	// 시야 매니저
	UPROPERTY(BlueprintReadOnly, Category = "Fog Of War")
	AFOW_Manager* FogOfWarManager;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TSubclassOf<AFOW_Manager> FogOfWarManagerClass;

	// 플레이어 ID 와 플레이어가 선택한 함선의 ID
	UPROPERTY(EditAnywhere, Category = "Player Info")
	TMap<FString, FPlayerInfo> RedTeamPlayerInfo;
	UPROPERTY(EditAnywhere, Category = "Player Info")
	TMap<FString, FPlayerInfo> BlueTeamPlayerInfo;

	UDataTable* BattleShipData;
	UDataTable* AircraftData;
};
