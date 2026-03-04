
#include "Game/WHGameMode.h"
#include "Game/WHGameState.h"
#include "BattleShip/WHBattleShipBase.h"
#include "FogOfWar/FOW_Manager.h"
#include "Kismet/GameplayStatics.h"

AWHGameMode::AWHGameMode()
{

}

void AWHGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 맵 정보
	
	// 픽 정보


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
	if (RedTeamBattleShipClasses.Num() > 0)
	{
		for (int32 i = 0; i < RedTeamBattleShipClasses.Num(); ++i)
		{
			if (RedTeamBattleShipClasses.IsValidIndex(i))
			{
				FVector SpawnLocation = FVector::ZeroVector;
				FRotator SpawnRotation = FRotator::ZeroRotator;
				FActorSpawnParameters SpawnParams;
				if (RedTeamControllers.IsValidIndex(i))
				{
					SpawnParams.Owner = RedTeamControllers[i];
				}
				else
				{
					SpawnParams.Owner = nullptr;
				}
				AWHBattleShipBase* NewBattleShip = GetWorld()->SpawnActorDeferred<AWHBattleShipBase>(RedTeamBattleShipClasses[i], FTransform(SpawnRotation, SpawnLocation));
				NewBattleShip->TeamType = ETeamType::Red;
				NewBattleShip->RevealerType = ERevealerType::BattleShip;
				NewBattleShip->SightRange = 10000;
				UGameplayStatics::FinishSpawningActor(NewBattleShip, FTransform(SpawnRotation, SpawnLocation));
				RedTeamBattleShips.Add(NewBattleShip);
			}
		}
	}

	if(BlueTeamBattleShipClasses.Num() > 0)
	{
		for (int32 i = 0; i < BlueTeamBattleShipClasses.Num(); ++i)
		{
			if (BlueTeamBattleShipClasses.IsValidIndex(i))
			{
				FVector SpawnLocation = FVector::ZeroVector;
				FRotator SpawnRotation = FRotator::ZeroRotator;
				FActorSpawnParameters SpawnParams;
				if (BlueTeamControllers.IsValidIndex(i))
				{
					SpawnParams.Owner = BlueTeamControllers[i];
				}
				else
				{
					SpawnParams.Owner = nullptr;
				}
				AWHBattleShipBase* NewBattleShip = GetWorld()->SpawnActorDeferred<AWHBattleShipBase>(BlueTeamBattleShipClasses[i], FTransform(SpawnRotation, SpawnLocation));
				NewBattleShip->TeamType = ETeamType::Blue;
				NewBattleShip->RevealerType = ERevealerType::BattleShip;
				NewBattleShip->SightRange = 10000;
				UGameplayStatics::FinishSpawningActor(NewBattleShip, FTransform(SpawnRotation, SpawnLocation));
				BlueTeamBattleShips.Add(NewBattleShip);
			}
		}
	}
}

void AWHGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 플레이어에 맞는 함선 할당
}
