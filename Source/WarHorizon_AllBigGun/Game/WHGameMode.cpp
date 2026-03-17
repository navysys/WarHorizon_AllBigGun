
#include "WarHorizon_AllBigGun/Game/WHGameMode.h"
#include "WarHorizon_AllBigGun/Game/WHGameState.h"
#include "WarHorizon_AllBigGun/Game/WHGameInstance.h"
#include "WarHorizon_AllBigGun/Controller/WHPlayerController.h"
#include "WarHorizon_AllBigGun/BattleShip/WHBattleShipBase.h"
#include "WarHorizon_AllBigGun/FogOfWar/FOW_Manager.h"
#include "Kismet/GameplayStatics.h"

AWHGameMode::AWHGameMode()
{

}

void AWHGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 맵 정보
	
	// 플레이어 정보 (웹서버에서 가져오기)
	// 
	// 데이터베이스 초기화
	UWHGameInstance* GI = Cast<UWHGameInstance>(GetGameInstance());
	if (GI)
	{
		BattleShipData = GI->GetBattleShipDataTable();
		AircraftData = GI->GetAircraftDataTable();
	}
	// 테스트용 데이터
	RedTeamPlayerInfo.Add(TEXT("Player1"), FPlayerInfo(nullptr, 14001, nullptr, nullptr));
	// 플레이어 인포 기반 함선 클래스 설정
	SetPlayerInfoBattleShipClass(RedTeamPlayerInfo);
	SetPlayerInfoBattleShipClass(BlueTeamPlayerInfo);
}

void AWHGameMode::BeginPlay()
{
	Super::BeginPlay();

	AWHGameState* WHGameState = GetGameState<AWHGameState>();
	check(WHGameState);
	
	// 시야 매니저 생성
	if (FogOfWarManagerClass)
	{
		FogOfWarManager = GetWorld()->SpawnActor<AFOW_Manager>(FogOfWarManagerClass);
	}

	// 함선	스폰
	if (!RedTeamPlayerInfo.IsEmpty())
	{
		for (auto& Elem : RedTeamPlayerInfo)
		{
			if (Elem.Value.BattleShipClass != nullptr)
			{
				// 스폰 위치와 회전은 임시로 설정, 나중에 맵 정보 기반으로 설정 필요
				FVector SpawnLocation = FVector::ZeroVector;
				FRotator SpawnRotation = FRotator::ZeroRotator;
				FActorSpawnParameters SpawnParams;
				if (IsValid(Elem.Value.PlayerController))
				{
					SpawnParams.Owner = Elem.Value.PlayerController;
				}
				else
				{
					SpawnParams.Owner = nullptr;
				}
				AWHBattleShipBase* NewBattleShip = GetWorld()->SpawnActorDeferred<AWHBattleShipBase>(Elem.Value.BattleShipClass, FTransform(SpawnRotation, SpawnLocation));
				NewBattleShip->TeamType = ETeamType::Red;
				NewBattleShip->RevealerType = ERevealerType::BattleShip;
				NewBattleShip->SightRange = 10000;
				UGameplayStatics::FinishSpawningActor(NewBattleShip, FTransform(SpawnRotation, SpawnLocation));
				Elem.Value.BattleShip = NewBattleShip;
			}
		}
	}

	if (!BlueTeamPlayerInfo.IsEmpty())
	{
		for (auto& Elem : BlueTeamPlayerInfo)
		{
			if (Elem.Value.BattleShipClass != nullptr)
			{
				// 스폰 위치와 회전은 임시로 설정, 나중에 맵 정보 기반으로 설정 필요
				FVector SpawnLocation = FVector::ZeroVector;
				FRotator SpawnRotation = FRotator::ZeroRotator;
				FActorSpawnParameters SpawnParams;

				AWHBattleShipBase* NewBattleShip = GetWorld()->SpawnActorDeferred<AWHBattleShipBase>(Elem.Value.BattleShipClass, FTransform(SpawnRotation, SpawnLocation));
				NewBattleShip->TeamType = ETeamType::Blue;
				NewBattleShip->RevealerType = ERevealerType::BattleShip;
				NewBattleShip->SightRange = 10000;
				UGameplayStatics::FinishSpawningActor(NewBattleShip, FTransform(SpawnRotation, SpawnLocation));
				Elem.Value.BattleShip = NewBattleShip;
			}
		}
	}
}

void AWHGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	// 플레이어 로그인 허용 여부 판단
	FString PlayerId = UGameplayStatics::ParseOption(Options, TEXT("PlayerId"));
	FString Team = UGameplayStatics::ParseOption(Options, TEXT("Team"));
	if (Team == "Red")
	{
		if (!RedTeamPlayerInfo.Contains(PlayerId))
		{
			ErrorMessage = TEXT("Unauthorized Player");
		}
	}
	else if (Team == "Blue")
	{
		if (!BlueTeamPlayerInfo.Contains(PlayerId))
		{
			ErrorMessage = TEXT("Unauthorized Player");
		}
	}
}

APlayerController* AWHGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{

	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

FString AWHGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	// 플레이어 컨트롤러 초기화

	FString PlayerId = UGameplayStatics::ParseOption(Options, TEXT("PlayerId"));
	FString Team = UGameplayStatics::ParseOption(Options, TEXT("Team"));

	AWHPlayerController* PC = Cast<AWHPlayerController>(NewPlayerController);
	if (PC)
	{
		PC->PlayerId = PlayerId;
		PC->Team = Team;

		if (Team == "Red")
		{
			FPlayerInfo* PlayerInfo = RedTeamPlayerInfo.Find(PlayerId);
			PlayerInfo->PlayerController = PC;
			if(IsValid(PlayerInfo->BattleShip))
			{
				PC->BattleShipPawn = PlayerInfo->BattleShip;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player %s's battleship is not valid."), *PlayerId);
			}
		}
		else if (Team == "Blue")
		{
			FPlayerInfo* PlayerInfo = RedTeamPlayerInfo.Find(PlayerId);
			BlueTeamPlayerInfo.Find(PlayerId)->PlayerController = PC;
			if (IsValid(PlayerInfo->BattleShip))
			{
				PC->BattleShipPawn = PlayerInfo->BattleShip;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player %s's battleship is not valid."), *PlayerId);
				// 함선 스폰 UI 표시하도록 해야 함

			}
		}
	}

	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

void AWHGameMode::SetPlayerInfoBattleShipClass(TMap<FString, FPlayerInfo>& PlayerInfo)
{
	if (BattleShipData)
	{
		for (auto& Elem : PlayerInfo)
		{
			uint16 BattleShipId = Elem.Value.BattleShipID;
			FName Id = FName(*FString::FromInt(BattleShipId));
			FBattleShipDataTable* Table = BattleShipData->FindRow<FBattleShipDataTable>(Id, "");
			if (Table)
			{
				Elem.Value.BattleShipClass = Table->BattleShipClass;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BattleShipDataTable does not contain data for BattleShipID: %d"), BattleShipId);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BattleShipData is not valid."));
	}
}