// Fill out your copyright notice in the Description page of Project Settings.


#include "WHAircraftsBase.h"
#include "WHCAircraftsMovement.h"
#include "WHAircraft.h"

// Sets default values
AWHAircraftsBase::AWHAircraftsBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMeshObject(TEXT("StaticMesh'/Game/test/fluzeug'"));
	if (DefaultMeshObject.Succeeded())
	{
		StaticMeshRef = DefaultMeshObject.Object;
	}
	AircraftMovementComp = CreateDefaultSubobject<UWHCAircraftsMovement>("AircraftMovementComp");

}

// Called when the game starts or when spawned
void AWHAircraftsBase::BeginPlay()
{
	Super::BeginPlay();
	
	AddAircraft(StaticMeshRef);
	AddAircraft(StaticMeshRef);
	AddAircraft(StaticMeshRef);
}

// Called every frame
void AWHAircraftsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveFront();
	SetPlaneFormations();
}

// Called to bind functionality to input
void AWHAircraftsBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWHAircraftsBase::MoveFront()
{
	AircraftMovementComp->AddInputVector(-GetActorForwardVector()); // 현재 후진하도록 설정
}

void AWHAircraftsBase::IncreaseHeight()
{

}

void AWHAircraftsBase::DecreaseHeight()
{
}

void AWHAircraftsBase::GunAttack()
{
}

void AWHAircraftsBase::BombAttack()
{
}

void AWHAircraftsBase::ChangeMovePoint()
{

}

void AWHAircraftsBase::AddAircraft(UStaticMesh* StaticMesh)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//임시 위치 조정
	AWHAircraft* SpawnedAircraft = GetWorld()->SpawnActor<AWHAircraft>(FVector(GetActorLocation().X, GetActorLocation().Y + ArrayAircrafts.Num() * 800, GetActorLocation().Z), FRotator::ZeroRotator, SpawnParams);
	SpawnedAircraft->StaticMeshComp->SetStaticMesh(StaticMesh);
	ArrayAircrafts.Emplace(SpawnedAircraft);
	AircraftsNum = ArrayAircrafts.Num();
}

void AWHAircraftsBase::DeleteAircraft()
{

}

void AWHAircraftsBase::SetPlaneFormations()
{
	FVector Location = GetActorLocation();
	PlaneFormations.Empty();
	if (AircraftsNum == 1)
	{
		PlaneFormations.Emplace(Location);
	}
	else if (AircraftsNum == 2)
	{
		PlaneFormations.Emplace(FVector(Location.X, Location.Y - 400.0f, Location.Z));
		PlaneFormations.Emplace(FVector(Location.X, Location.Y + 400.0f, Location.Z));
	}
	else if (AircraftsNum == 3)
	{
		PlaneFormations.Emplace(Location);
		PlaneFormations.Emplace(FVector(Location.X - 500.0f, Location.Y - 400.0f, Location.Z));
		PlaneFormations.Emplace(FVector(Location.X - 500.0f, Location.Y + 400.0f, Location.Z));
	}
	for (int i = 0; i < ArrayAircrafts.Num(); i++)
	{
		ArrayAircrafts[i]->CurrentPosition = PlaneFormations[i];
	}
	
}

void AWHAircraftsBase::SetTargetAircraft()
{

}

void AWHAircraftsBase::SetTargetBattleShip()
{

}
