
#include "Turret/WHTurretBase.h"
#include "Game/WHGameInstance.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"
#include "Turret/WHShell.h"
#include "Engine/StaticMeshSocket.h"


AWHTurretBase::AWHTurretBase()
{
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

void AWHTurretBase::BeginPlay()
{
	Super::BeginPlay();
	
	BaseBattleShip = GetInstigator();
	
	GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &AWHTurretBase::SpinToTargetAngle, RotationDelay, false);
}

void AWHTurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BeforeFireTime += DeltaTime;
	if (!bIsFireReady)
	{
		if (bIsLookTarget)
		{
			if (BeforeFireTime > ReloadTime)
			{
				bIsFireReady = true;
			}
		}
	}

	DebugTurretForward();
}

void AWHTurretBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(StaticMeshComp))
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
	bIsFireReady = false;
	BeforeFireTime = 0.0f;
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
				RotationDelay = Table->TurretRotationSpeed;
			}
		}
	}
}

void AWHTurretBase::DebugTurretForward()
{
	FVector TurretLocation = StaticMeshComp->GetComponentLocation();
	FVector ForwardVector = StaticMeshComp->GetForwardVector();
	FVector LineEnd = TurretLocation + ForwardVector * Range;
	FColor Color = FColor::Yellow;
	float Thickness = 10.0f;

	if (TurretType == ETurretType::Main)
	{
		Color = FColor::Red;
		Thickness = 100.0f;
	}
	else if (TurretType == ETurretType::Sub)
	{
		Color = FColor::Yellow;
		Thickness = 50.0f;
	}
	else if (TurretType == ETurretType::Air)
	{
		Color = FColor::Blue;
		Thickness = 20.0f;
	}
	else if (TurretType == ETurretType::DualPurpose)
	{
		Color = FColor::Green;
		Thickness = 50.0f;
	}

	DrawDebugLine(GetWorld(), TurretLocation, LineEnd, Color, false, -1.f, 0, Thickness);
}

void AWHTurretBase::SpinToTargetAngle()
{
	if (IsValid(StaticMeshComp) && IsValid(BaseBattleShip))
	{
		float TurretYaw = StaticMeshComp->GetRelativeRotation().Yaw;
		
		if (MaxHorizontalAngle == 360.0f)
		{
			float Angle;
			// 360도 회전 계산용 각도
			if (TargetData.Angle == 0)
			{
				Angle = 0.0f;
			}
			else
			{
				Angle = TurretYaw + TargetData.Angle - SocketYaw;
			}

			if (Angle > 180.0f)
			{
				Angle -= 360.0f;
			}
			else if (Angle < -180.0f)
			{
				Angle += 360.0f;
			}

			if (Angle > 1.0f)
			{
				StaticMeshComp->AddRelativeRotation(FRotator(0, 0.5f, 0));
			}
			else if (Angle < -1.0f)
			{
				StaticMeshComp->AddRelativeRotation(FRotator(0, -0.5f, 0));
			}
		}
		else
		{
			float RelativeAngle;
			// 제한된 각도 내에서의 회전 각도
			if (TargetData.Angle == 0)
			{
				RelativeAngle = 0.0f;
			}
			else
			{
				RelativeAngle = round(TargetData.Angle - SocketYaw);
			}

			if (RelativeAngle > 180.0f)
			{
				RelativeAngle -= 360.0f;
			}
			else if (RelativeAngle < -180.0f)
			{
				RelativeAngle += 360.0f;
			}

			if (abs(RelativeAngle) < MaxHorizontalAngle)
			{
				if (TurretYaw > RelativeAngle + 1.0f)
				{
					StaticMeshComp->AddRelativeRotation(FRotator(0, -0.5f, 0));
					if (StaticMeshComp->GetRelativeRotation().Yaw < RelativeAngle)
					{
						StaticMeshComp->SetRelativeRotation(FRotator(0, RelativeAngle, 0));
					}
				}
				else if (TurretYaw < RelativeAngle - 1.0f)
				{
					StaticMeshComp->AddRelativeRotation(FRotator(0, 0.5f, 0));
					if (StaticMeshComp->GetRelativeRotation().Yaw > RelativeAngle)
					{
						StaticMeshComp->SetRelativeRotation(FRotator(0, RelativeAngle, 0));
					}
				}
				else
				{
					StaticMeshComp->SetRelativeRotation(FRotator(0, RelativeAngle, 0));
				}
			}
			else
			{
				if (abs(TurretYaw) < MaxHorizontalAngle)
				{
					if (TurretYaw >= 0)
					{
						StaticMeshComp->AddRelativeRotation(FRotator(0, 0.5f, 0));
					}
					else
					{
						StaticMeshComp->AddRelativeRotation(FRotator(0, -0.5f, 0));
					}
				}
				else
				{
					StaticMeshComp->SetRelativeRotation(FRotator(0, MaxHorizontalAngle, 0));
				}
			}
		}
		if (TargetData.Angle != 0 && TargetData.Distance != 0)
		{
			float RAngle = round(TargetData.Angle - SocketYaw);
			if (RAngle > 180.0f)
			{
				RAngle -= 360.0f;
			}
			else if (RAngle < -180.0f)
			{
				RAngle += 360.0f;
			}

			if (abs(TurretYaw - RAngle) < 5.0f)
			{
				bIsLookTarget = true;
			}
			else
			{
				bIsLookTarget = false;
			}
		}

		GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &AWHTurretBase::SpinToTargetAngle, RotationDelay, false);
	}
}

void AWHTurretBase::SetTargetData(FTargetData Data)
{
	TargetData = Data;
}

void AWHTurretBase::SetTargetData(const TArray<FTargetData>* DatasPtr)
{
	if (DatasPtr)
	{
		for (FTargetData Data : *DatasPtr)
		{
			float RelativeAngle = Data.Angle - SocketYaw;
			if (RelativeAngle > 180.0f)
			{
				RelativeAngle -= 360.0f;
			}
			else if (RelativeAngle < -180.0f)
			{
				RelativeAngle += 360.0f;
			}

			if (abs(RelativeAngle) < MaxHorizontalAngle)
			{
				TargetData = Data;
				return;
			}
		}
	}
}
