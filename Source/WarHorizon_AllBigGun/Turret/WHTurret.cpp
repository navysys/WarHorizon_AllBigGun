// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret/WHTurret.h"
#include "DataTable/TurretDataTable.h"
#include "Game/WHGameSingleton.h"
#include "Controller/WHTurretAIController.h"
//#include "ProjectileShell.h"
#include "Engine/StaticMeshSocket.h"

// Sets default values
AWHTurret::AWHTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	AIControllerClass = AWHTurretAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AWHTurret::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWHTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWHTurret::Fire()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator Rot = GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f);
	for (int i = 0; i < Muzzles.Num(); i++)
	{
		//GetWorld()->SpawnActor<AProjectileShell>(GetActorLocation() + GetActorRotation().RotateVector(Muzzles[i]->RelativeLocation), Rot, SpawnParams);
	}
}

void AWHTurret::LoadDataTableToName(FName Name)
{
	UDataTable* TurretData = UWHGameSingleton::Get().GetTurretDataTable();
	FTurretDataTable* Table = TurretData->FindRow<FTurretDataTable>(Name, "");

	if (Table)
	{
		TurretID = Table->TurretID;
		TurretName = Table->TurretName;
		TurretType = Table->TurretType;
		TurretMesh = Table->TurretMesh;

		MaxAmmo = Table->Ammo;
		ReloadTime = Table->ReloadTime;
		Range = Table->Range;

		HorizontalAngle = Table->HorizontalAngle;
		VerticalAngle = Table->VerticalAngle;
		RotationSpeed = Table->RotationSpeed;

		InitStat();
	}
}

void AWHTurret::InitStat()
{
	StaticMeshComp->SetStaticMesh(TurretMesh);
	SetMuzzles(StaticMeshComp);
	Ammo = MaxAmmo;

}

void AWHTurret::SetupTurret(APawn* BaseShip, FName Name)
{
	BaseBattleShip = BaseShip;
	LoadDataTableToName(Name);
}

void AWHTurret::SetMuzzles(UStaticMeshComponent* MeshComp)
{
	Muzzles = MeshComp->GetStaticMesh()->Sockets;
}


APawn* AWHTurret::GetBaseBattleShip()
{
	return BaseBattleShip;
}

APawn* AWHTurret::GetTurretTarget()
{
	return Target;
}

float AWHTurret::GetRotationSpeed()
{
	return RotationSpeed;
}

float AWHTurret::GetReloadTime()
{
	return ReloadTime;
}

uint16 AWHTurret::GetAmmo()
{
	return Ammo;
}

