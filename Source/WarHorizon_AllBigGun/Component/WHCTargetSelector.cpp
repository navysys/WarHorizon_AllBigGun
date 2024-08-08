// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WHCTargetSelector.h"
#include "Turret/WHTurret.h"

// Sets default values for this component's properties
UWHCTargetSelector::UWHCTargetSelector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AirTurretRotationSpeed = 10.0f;
	// ...
}


// Called when the game starts
void UWHCTargetSelector::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWHCTargetSelector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsActiveAirTurrets)
	{
		if (CurrentFowardAngle != 999.0f)
		{
			ChangeAirTurretRotation(FowardAttackAirTurrets, CurrentFowardAngle, DeltaTime);
		}
		if (CurrentBackAngle != 999.0f)
		{
			ChangeAirTurretRotation(BackAttackAirTurrets, CurrentBackAngle, DeltaTime);
		}
		if (CurrentLeftAngle != 999.0f)
		{
			ChangeAirTurretRotation(LeftAttackAirTurrets, CurrentLeftAngle, DeltaTime);
		}
		if (CurrentRightAngle != 999.0f)
		{
			ChangeAirTurretRotation(RightAttackAirTurrets, CurrentRightAngle, DeltaTime);
		}
	}
}

void UWHCTargetSelector::InitTargetSelectorComponent(const TArray<FTurretArray> AllArray, TArray<APawn*> BattleShips, TArray<APawn*> Aircrafts)
{
	AllTurretArray = AllArray;
	BSTargetArray = BattleShips;
	ACTargetArray = Aircrafts;

	for (FTurretArray TArray : AllTurretArray)
	{
		if (TArray.TurretsType == ETurretType::Air)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				AirTurretArray.Emplace(Turret);
			}
		}
	}
	AirTurretArrayNum = AirTurretArray.Num();
	IsActiveAirTurrets = true;

	for (AWHTurretBase* Turret : AirTurretArray)
	{
		char Dir = Turret->GetFrontDirection();
		if (Dir == 'L')
		{
			LeftAttackAirTurrets.Emplace(Turret);
		}
		else if (Dir == 'R')
		{
			RightAttackAirTurrets.Emplace(Turret);
		}
		else if (Dir == 'F')
		{
			FowardAttackAirTurrets.Emplace(Turret);
		}
		else if (Dir == 'B')
		{
			BackAttackAirTurrets.Emplace(Turret);
		}
	}

	SetTurretsAttackType(ETurretType::Sub, ETurretAttackType::TargetAttack);
	SetTurretsAttackType(ETurretType::DualPurpose, ETurretAttackType::TargetAttack);
}

void UWHCTargetSelector::SetTurretsAttackType(ETurretType TurretType, ETurretAttackType Type)
{
	for (FTurretArray TArray : AllTurretArray)
	{
		if (TArray.TurretsType == TurretType)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				Cast<AWHTurret>(Turret)->SetAttackType(Type);
			}
		}
	}
}

void UWHCTargetSelector::SetTurretsTargetDistance(ETurretType TurretType, float Dist)
{
	for (FTurretArray TArray : AllTurretArray)
	{
		if (TArray.TurretsType == TurretType)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				Cast<AWHTurret>(Turret)->SetTargetDistance(Dist);
			}
		}
	}
}

void UWHCTargetSelector::SetTurretsTarget(ETurretType TurretType, APawn* Pawn)
{
	for (FTurretArray TArray : AllTurretArray)
	{
		if (TArray.TurretsType == TurretType)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				Cast<AWHTurret>(Turret)->SetTargetPawn(Pawn);
			}
		}
	}
}

void UWHCTargetSelector::SetTurretsTarget(ETurretType TurretType, FVector Point)
{
	for (FTurretArray TArray : AllTurretArray)
	{
		if (TArray.TurretsType == TurretType)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				Cast<AWHTurret>(Turret)->SetTargetPoint(Point);
			}
		}
	}
}

void UWHCTargetSelector::SetTurretsTarget(ETurretType TurretType, float Angle)
{
	for (FTurretArray TArray : AllTurretArray)
	{
		if (TArray.TurretsType == TurretType)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				Cast<AWHTurret>(Turret)->SetTargetAngle(Angle);
			}
		}
	}
}

void UWHCTargetSelector::CommandTurretsFire(ETurretType TurretType)
{
	for (FTurretArray TArray : AllTurretArray)
	{
		if (TArray.TurretsType == TurretType)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				Cast<AWHTurret>(Turret)->Fire();
			}
		}
	}
}

void UWHCTargetSelector::CalculateAirTurretRotation()
{
	ACTargetAngles.Empty();
	if (ACTargetArray.Num() != 0)
	{
		AActor* Owner = GetOwner();

		for (APawn* AC : ACTargetArray)
		{
			FVector Dir1 = AC->GetActorLocation() - Owner->GetActorLocation();
			Dir1.Normalize();
			FVector Dir2 = Owner->GetActorForwardVector();

			float DotProduct = FVector::DotProduct(Dir1, Dir2);
			float Angle = FMath::Acos(DotProduct) * (180.0f / PI);

			ACTargetAngles.Emplace(Angle);
		}
		
		if (ACTargetAngles.Num() != 0)
		{
			bool L = true;
			bool R = true;
			bool F = true;
			bool B = true;
			for (int i = 0; i < ACTargetAngles.Num(); i++)
			{
				if (ACTargetAngles[i] <= -30.0f && ACTargetAngles[i] >= -150.0f)
				{
					if (L)
					{
						CurrentLeftAngle = ACTargetAngles[i] - 90.0f;
						L = false;
					}
				}
				else if (ACTargetAngles[i] >= 30.0f && ACTargetAngles[i] <= 150.0f)
				{
					if (R)
					{
						CurrentRightAngle = ACTargetAngles[i] + 90.0f;
						R = false;
					}
				}
				else if (ACTargetAngles[i] > -30.0f && ACTargetAngles[i] < 30.0f)
				{
					if (F)
					{
						CurrentFowardAngle = ACTargetAngles[i];
						F = false;
					}
				}
				else if (ACTargetAngles[i] > 150.0f && ACTargetAngles[i] < -150.0f)
				{
					if (B)
					{
						float InitAngle = 0;
						if (ACTargetAngles[i] > 0)
						{
							InitAngle = -180.0f;
						}
						else
						{
							InitAngle = 180.0f;
						}
						CurrentBackAngle = ACTargetAngles[i] + InitAngle;
						B = false;
					}
				}
			}
		}

	}
}

void UWHCTargetSelector::ChangeAirTurretRotation(TArray<AWHTurretBase*> TurretDir, float CurrentAngle , float DeltaTime)
{
	float TurretAngle = round(TurretDir[0]->GetActorRotation().Yaw);
	UE_LOG(LogTemp, Warning, TEXT("%f"), TurretAngle);
	if (CurrentAngle > TurretAngle)
	{
		FRotator Rot = FRotator(0.0f, AirTurretRotationSpeed * DeltaTime, 0.0f);
		for (AWHTurretBase* Turret : TurretDir)
		{
			Turret->AddActorLocalRotation(Rot);
		}
	}
	else if (CurrentAngle < TurretAngle)
	{
		FRotator Rot = FRotator(0.0f, -AirTurretRotationSpeed * DeltaTime, 0.0f);
		for (AWHTurretBase* Turret : TurretDir)
		{
			Turret->AddActorLocalRotation(Rot);
		}
	}
}

