// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret/WHTurretBase.h"
#include "Game/WHGameInstance.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"
#include "Turret/WHShell.h"
#include "Engine/StaticMeshSocket.h"


// Sets default values
AWHTurretBase::AWHTurretBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	for (int i = 0; i < 4; i++)
	{
		FName CompName = FName(TEXT("Muzzle"),i+1);
		USceneComponent* Muzzle = CreateDefaultSubobject<USceneComponent>(CompName);
		Muzzle->SetupAttachment(StaticMeshComp);
		MuzzleComps.Emplace(Muzzle);
	}
	
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

	DebugTurretForward();
	SpinToTargetAngle();
}

void AWHTurretBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (StaticMeshComp != nullptr)
	{
		TArray<UStaticMeshSocket*> Muzzles = StaticMeshComp->GetStaticMesh()->Sockets;

		for (int i = 0; i < Muzzles.Num(); i++)
		{
			MuzzleComps[i]->SetRelativeLocation(Muzzles[i]->RelativeLocation);
		}
		if (Muzzles.Num() < MuzzleComps.Num())
		{
			int OverNum = MuzzleComps.Num() - Muzzles.Num();
			for (int j = 0; j < OverNum; j++)
			{
				MuzzleComps.Pop()->DestroyComponent();
			}
		}

		// 분산도 관련 코드 나중에는 계수 곱해주도록 수정
		int MuzzleInt = MuzzleComps.Num();
		float DispersionAngle = 0.0f;
		if (MuzzleInt == 4)
		{
			DispersionAngle = 7.5f;
		}
		else if (MuzzleInt == 3)
		{
			DispersionAngle = 5.0f;
		}
		else if (MuzzleInt == 2)
		{
			DispersionAngle = 2.5f;
		}

		for (int i = 1; i <= MuzzleInt; i++)
		{
			Dispersion.Emplace((DispersionAngle / (MuzzleInt - 1)) * (i - ((MuzzleInt + 1) / 2)));
		}
	}
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

void AWHTurretBase::Fire()
{
	if (GunFireEffect)
	{
		float Num = MuzzleComps.Num() / 2;
		int Center = FMath::CeilToInt(Num);
		ANiagaraActor* NiagaraActor = GetWorld()->SpawnActor<ANiagaraActor>(ANiagaraActor::StaticClass(), MuzzleComps[Center]->GetComponentLocation(), StaticMeshComp->GetRelativeRotation());
		if (NiagaraActor)
		{
			NiagaraActor->GetNiagaraComponent()->SetAsset(GunFireEffect);
			NiagaraActor->GetNiagaraComponent()->Activate(true);
		}
	}
}

void AWHTurretBase::LoadDataTableToName(FName Name)
{
	UWHGameInstance* GI = Cast<UWHGameInstance>(GetGameInstance());
	if (GI)
	{
		UDataTable* TurretData = GI->GetTurretDataTable();
		if (TurretData)
		{
			FTurretDataTable* Table = TurretData->FindRow<FTurretDataTable>(Name, "");

			if (Table)
			{
				TurretID = Table->TurretID;
				TurretName = Table->TurretName;
				TurretType = Table->TurretType;

				ReloadTime = Table->TurretReloadTime;
				Range = Table->TurretRange;

				MaxHorizontalAngle = Table->TurretHorizontalAngle;
				MaxVerticalAngle = Table->TurretVerticalAngle;
				RotationSpeed = Table->TurretRotationSpeed;
			}
		}
	}
}

void AWHTurretBase::DebugTurretForward()
{
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

bool AWHTurretBase::SpinToTargetAngle()
{
	if (StaticMeshComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), TargetAngle);
		//FRotator Rot = StaticMeshComp->GetRelativeRotation();
		//StaticMeshComp->AddLocalRotation(FRotator(0, 1, 0));

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Rot.ToString());
	}


	return false;
}

void AWHTurretBase::SetTargetDistance(float Distance)
{
	TargetDistance = Distance;
}

void AWHTurretBase::SetTargetAngle(float Angle)
{
	TargetAngle = Angle;
}
