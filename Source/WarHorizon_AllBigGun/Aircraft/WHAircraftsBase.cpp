// Fill out your copyright notice in the Description page of Project Settings.


#include "Aircraft/WHAircraftsBase.h"
#include "Aircraft/WHAircraft.h"
#include "Component/WHCAircraftsMovement.h"
//#include ""

// Sets default values
AWHAircraftsBase::AWHAircraftsBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxSpeed = 1500.0f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMeshObject(TEXT("StaticMesh'/Game/Resource/AircraftTest/Aircraft_Type0'"));
	if (DefaultMeshObject.Succeeded())
	{
		StaticMeshRef = DefaultMeshObject.Object;
	}
	AircraftMovementComp = CreateDefaultSubobject<UWHCAircraftsMovement>("AircraftMovementComp");
	AircraftMovementComp->MaxSpeed = this->MaxSpeed;
	
}

// Called when the game starts or when spawned
void AWHAircraftsBase::BeginPlay()
{
	Super::BeginPlay();
	
	AddAircraft(FVector(0, 0, 0));
	AddAircraft(FVector(0, 0, 0));
	AddAircraft(FVector(0, 0, 0));
	AddAircraft(FVector(0, 0, 0));
}

// Called every frame
void AWHAircraftsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveFront();
	SetAircraftFormations();
}

// Called to bind functionality to input
void AWHAircraftsBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWHAircraftsBase::MoveFront()
{
	AircraftMovementComp->AddInputVector(GetActorForwardVector());
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

void AWHAircraftsBase::AddAircraft(FVector StartPos)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//임시 위치 조정
	AWHAircraft* SpawnedAircraft = GetWorld()->SpawnActor<AWHAircraft>(StartPos, FRotator::ZeroRotator, SpawnParams);
	SpawnedAircraft->StaticMeshComp->SetStaticMesh(StaticMeshRef);
	SpawnedAircraft->MaxSpeed = this->MaxSpeed;
	ArrayAircrafts.Emplace(SpawnedAircraft);
	AircraftsNum = ArrayAircrafts.Num();
	SetInitAircraftPosition();
}

TArray<AActor*> AWHAircraftsBase::GetArrayAircrafts()
{
	TArray<AActor*> ArrayAirs;
	for (AWHAircraft* air : ArrayAircrafts)
	{
		ArrayAirs.Emplace(air);
	}

	return ArrayAirs;
}

uint8 AWHAircraftsBase::GetAircraftType()
{
	return (uint8)AircraftType;
}

void AWHAircraftsBase::DeleteAircraft()
{
	//아직 리스트 삭제는 구현X
	AircraftsNum = ArrayAircrafts.Num();
	SetInitAircraftPosition();
}

void AWHAircraftsBase::SetInitAircraftPosition()
{
	InitAircraftPosition.Empty();

	if (AircraftsNum == 1)
	{
		InitAircraftPosition.Emplace(FVector::ZeroVector);
	}
	else if (AircraftsNum == 2)
	{
		InitAircraftPosition.Emplace(FVector(0.0f, -400.0f, 0.0f));
		InitAircraftPosition.Emplace(FVector(0.0f, 400.0f, 0.0f));
	}
	else if (AircraftsNum == 3)
	{
		InitAircraftPosition.Emplace(FVector::ZeroVector);
		InitAircraftPosition.Emplace(FVector(-600.0f, -400.0f, 0.0f));
		InitAircraftPosition.Emplace(FVector(-600.0f, 400.0f, 0.0f));
	}
	else if (AircraftsNum == 4)
	{
		InitAircraftPosition.Emplace(FVector::ZeroVector);
		InitAircraftPosition.Emplace(FVector(-600.0f, -400.0f, 0.0f));
		InitAircraftPosition.Emplace(FVector(-600.0f, 400.0f, 0.0f));
		InitAircraftPosition.Emplace(FVector(-1200.0f, 0.0f, 0.0f));
	}
}

void AWHAircraftsBase::SetAircraftFormations()
{
	FVector Location = GetActorLocation();
	FRotator Rotator = GetActorRotation();
	
	AircraftFormations.Empty();
	if (InitAircraftPosition.Num() > 0)
	{
		for (int i = 0; i < (int)AircraftsNum; i++)
		{
			AircraftFormations.Emplace(Location + Rotator.RotateVector(InitAircraftPosition[i]));
		}

		if (ArrayAircrafts.Num() > 0)
		{
			for (int i = 0; i < ArrayAircrafts.Num(); i++)
			{
				ArrayAircrafts[i]->CurrentPosition = AircraftFormations[i];
			}
		}
	}
}

void AWHAircraftsBase::SetTargetAircraft()
{

}

void AWHAircraftsBase::SetTargetBattleShip()
{

}

void AWHAircraftsBase::Fire()
{

}
