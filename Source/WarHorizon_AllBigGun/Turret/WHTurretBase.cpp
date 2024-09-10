// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret/WHTurretBase.h"
#include "Game/WHGameInstance.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"


// Sets default values
AWHTurretBase::AWHTurretBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::Disabled;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetCollisionProfileName(TEXT("NoCollision"));


}

// Called when the game starts or when spawned
void AWHTurretBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWHTurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector TurretLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector LineEnd = GetActorLocation();
	FColor Color = FColor::Yellow;
	float Thickness = 10.0f;

	if (TurretType == ETurretType::Main)
	{
		LineEnd = TurretLocation + ForwardVector * Range;
		Color = FColor::Red;
		Thickness = 100.0f;
	}
	else if (TurretType == ETurretType::Sub)
	{
		LineEnd = TurretLocation + ForwardVector * Range;
		Color = FColor::Yellow;
		Thickness = 50.0f;
	}
	else if (TurretType == ETurretType::Air)
	{
		LineEnd = TurretLocation + ForwardVector * Range;
		Color = FColor::Blue;
		Thickness = 20.0f;
	}
	else if (TurretType == ETurretType::DualPurpose)
	{
		LineEnd = TurretLocation + ForwardVector * Range;
		Color = FColor::Green;
		Thickness = 50.0f;
	}

	DrawDebugLine(GetWorld(), TurretLocation, LineEnd, Color, false, -1.f, 0, Thickness);
}

void AWHTurretBase::SetFrontDirection(char Dir)
{
	FrontDirection = Dir;
	if (FrontDirection == 'L')
	{
		SocketYaw = -90.0f;
	}
	else if (FrontDirection == 'R')
	{
		SocketYaw = 90.0f;
	}
	else if (FrontDirection == 'F')
	{
		SocketYaw = 0.0f;
	}
	else if (FrontDirection == 'B')
	{
		SocketYaw = 180.0f;
	}
}

void AWHTurretBase::SetupTurret(APawn* BaseShip, FName Name)
{
	BaseBattleShip = BaseShip;
	LoadDataTableToName(Name);
}

void AWHTurretBase::Fire()
{
	if (GunFireEffect)
	{
		float Num = Muzzles.Num() / 2;
		int Center = FMath::CeilToInt(Num);
		ANiagaraActor* NiagaraActor = GetWorld()->SpawnActor<ANiagaraActor>(ANiagaraActor::StaticClass(), GetActorLocation() + GetActorRotation().RotateVector(Muzzles[Center - 1]->RelativeLocation), GetActorRotation());
		if (NiagaraActor)
		{
			NiagaraActor->GetNiagaraComponent()->SetAsset(GunFireEffect);
			NiagaraActor->GetNiagaraComponent()->Activate(true);
		}
	}
}

void AWHTurretBase::LoadDataTableToName(FName Name)
{
	UDataTable* TurretData = Cast<UWHGameInstance>(GetGameInstance())->GetTurretDataTable();
	FTurretDataTable* Table = TurretData->FindRow<FTurretDataTable>(Name, "");

	if (Table)
	{
		TurretID = Table->TurretID;
		TurretName = Table->TurretName;
		TurretType = Table->TurretType;
		TurretMesh = Table->TurretMesh;

		ReloadTime = Table->ReloadTime;
		Range = Table->Range;

		MaxHorizontalAngle = Table->HorizontalAngle;
		MaxVerticalAngle = Table->VerticalAngle;
		RotationSpeed = Table->RotationSpeed;

		if (StaticMeshComp != nullptr)
		{
			StaticMeshComp->SetStaticMesh(TurretMesh);
			Muzzles = StaticMeshComp->GetStaticMesh()->Sockets;
		}

	}
}