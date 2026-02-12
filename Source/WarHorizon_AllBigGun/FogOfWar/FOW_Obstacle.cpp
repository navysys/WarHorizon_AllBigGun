// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FOW_Obstacle.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/WHGameState.h"
#include "FogOfWar/FOW_Manager.h"
#include "FogOfWar/FOW_ClientManager.h"
#include "Game/WHGameMode.h"
#include "Controller/WHPlayerController.h"

// Sets default values
AFOW_Obstacle::AFOW_Obstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObstacleMesh"));
	RootComponent = ObstacleMesh;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
}

// Called when the game starts or when spawned
void AFOW_Obstacle::BeginPlay()
{
	Super::BeginPlay();
	
	bIsIncreasingSmoke = true;

	//매니저에 등록 - 서버에서만 작동
	if (HasAuthority())
	{
		AWHGameMode* GameMode = Cast<AWHGameMode>(UGameplayStatics::GetGameMode((GetWorld())));
		if (IsValid(GameMode))
		{
			FogManager = GameMode->GetFogOfWarManager();
			if (IsValid(FogManager))
			{
				FogManager->RegisterFOWObstacle(this);
				UE_LOG(LogTemp, Warning, TEXT("FogObstacle : ReisterFogObstacle"));
			}
		}
	}
	else
	{
		// 클라이언트 측 등록
		AWHPlayerController* PC = Cast<AWHPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (IsValid(PC))
		{
			AFOW_ClientManager* ClientManager = PC->FogOfWarClientManager;
			if (IsValid(ClientManager))
			{
				ClientManager->RegisterFOWObstacle(this);
				UE_LOG(LogTemp, Warning, TEXT("FogObstacle : ReisterFogObstacle Client"));
			}
		}
	}

}

void AFOW_Obstacle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// 매니저에서 제거 - 서버에서만 작동
	if (HasAuthority())
	{
		if (IsValid(FogManager))
		{
			FogManager->RemoveFOWObstacle(this);
			UE_LOG(LogTemp, Warning, TEXT("FogObstacle : RemoveFogObstacle"));
		}
	}
	else
	{
		// 클라이언트 측 제거
		AWHPlayerController* PC = Cast<AWHPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (IsValid(PC))
		{
			AFOW_ClientManager* ClientManager = PC->FogOfWarClientManager;
			if (IsValid(ClientManager))
			{
				ClientManager->RemoveFOWObstacle(this);
				UE_LOG(LogTemp, Warning, TEXT("FogObstacle : RemoveFogObstacle Client"));
			}
		}
	}
}

// Called every frame
void AFOW_Obstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	if (bIsIncreasingSmoke)
	{
		IncreaceSmoke(DeltaTime);
	}
	else
	{
		if (ElapsedTime >= LifeTime)
		{
			DecreaseSmoke(DeltaTime);
		}
	}
}

void AFOW_Obstacle::IncreaceSmoke(float DeltaTime)
{
	SmokeParameter += DeltaTime * SmokeDeploySpeed;

	if (SmokeParameter >= 100)
	{
		SmokeParameter = 100.0f;
		bIsIncreasingSmoke = false;
	}
}

void AFOW_Obstacle::DecreaseSmoke(float DeltaTime)
{
	SmokeParameter -= DeltaTime * SmokeDeploySpeed;

	if (SmokeParameter <= 0)
	{
		// 액터 파괴 전에 매니저에서 제거 작업 수행
		FogManager->RemoveFOWObstacle(this);
		Destroy();
	}
}

