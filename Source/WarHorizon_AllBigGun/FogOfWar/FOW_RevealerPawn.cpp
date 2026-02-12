// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWar/FOW_RevealerPawn.h"
#include "Net/UnrealNetwork.h"
#include "Game/WHGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "FogOfWar/FOW_Manager.h"

// Sets default values
AFOW_RevealerPawn::AFOW_RevealerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFOW_RevealerPawn::BeginPlay()
{
	Super::BeginPlay();

	RegisterToFogOfWarManager();
}

void AFOW_RevealerPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsValid(FogManager))
	{
		FogManager->RemoveBattleShipRevealer(this, TeamType);
	}
}

// Called every frame
void AFOW_RevealerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFOW_RevealerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFOW_RevealerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFOW_RevealerPawn, FOWVisibility);
}

void AFOW_RevealerPawn::OnRep_FOWVisibility()
{
	switch (FOWVisibility)
	{
	case EFOWVisibility::Visible:
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		break;
	case EFOWVisibility::Hidden:
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		break;
	}
}

void AFOW_RevealerPawn::RegisterToFogOfWarManager()
{
	//매니저에 등록 - 서버에서만 작동
	AWHGameMode* GameMode = Cast<AWHGameMode>(UGameplayStatics::GetGameMode((GetWorld())));
	if (IsValid(GameMode))
	{
		FogManager = GameMode->GetFogOfWarManager();
		if (IsValid(FogManager))
		{
			FogManager->AddBattleShipRevealer(this, SightRange, TeamType);
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("WHBattleShipBase : BeginPlay -> RegisterToFogOfWarManager Failed"));
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AFOW_RevealerPawn::RegisterToFogOfWarManager);
}