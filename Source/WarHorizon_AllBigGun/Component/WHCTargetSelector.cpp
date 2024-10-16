
#include "Component/WHCTargetSelector.h"
#include "Turret/WHTurretBase.h"



UWHCTargetSelector::UWHCTargetSelector()
{
	PrimaryComponentTick.bCanEverTick = true;

	AirTurretRotationSpeed = 10.0f;
}


// Called when the game starts
void UWHCTargetSelector::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = GetOwner();
}


// Called every frame
void UWHCTargetSelector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsActiveAirTurrets)
	{
		if (CurrentFowardAngle != 9999.9999f)
		{
			ChangeAirTurretRotation(FowardAttackAirTurrets, CurrentFowardAngle, DeltaTime);
		}
		if (CurrentBackAngle != 9999.9999f)
		{
			ChangeAirTurretRotation(BackAttackAirTurrets, CurrentBackAngle, DeltaTime);
		}
		if (CurrentLeftAngle != 9999.9999f)
		{
			ChangeAirTurretRotation(LeftAttackAirTurrets, CurrentLeftAngle, DeltaTime);
		}
		if (CurrentRightAngle != 9999.9999f)
		{
			ChangeAirTurretRotation(RightAttackAirTurrets, CurrentRightAngle, DeltaTime);
		}
	}

	if (bIsTracingTarget)
	{
		if (IsValid(MainTarget))
		{
			// �Լ��� Ÿ�� ������ ������ �Ÿ� ��� �� ����
			FVector Pos = Owner->GetActorLocation();
			FVector TPos = MainTarget->GetActorLocation();

			float Dist = FVector::Dist2D(Pos, TPos);
			
			FVector Dir = (TPos - Pos).GetSafeNormal();
			float Angle = FRotationMatrix::MakeFromX(Dir).Rotator().Yaw;

			if (Angle < 0)
			{
				Angle += 360.0f;
			}


			for (FTurretArray TArray : AllTurretArray)
			{
				if (TArray.TurretsType == ETurretType::Main)
				{
					for (AWHTurretBase* Turret : TArray.Turrets)
					{
						Turret->SetTargetAngle(Angle);
						Turret->SetTargetDistance(Dist);
					}
				}
				
			}
		}
	}
}

void UWHCTargetSelector::CalculateToTargetInfo()
{
	for (FTurretArray TArray : AllTurretArray)
	{
		switch (TArray.TurretAttackType)
		{
		case ETurretAttackType::Wait:
			break;
		case ETurretAttackType::PointAttack:

			break;
		case ETurretAttackType::TargetAttack:

			break;
		case ETurretAttackType::Invalid:
			break;
		default:
			break;
		}
	}
	// Main �ͷ��� ���� ��Ʈ�ѷ��� ��ǲ�� �޾� Point �� ��ǥ�� �����ϰų� Target �� Ʈ��ŷ�ϰų� ������ �� �־�� ��
	// ���� �ð����� SUB , AIR �ͷ��� �迭�� �ִ� �� �߿� ������ �� �ִ� ���� �ڵ� �����ؾ���
	// Dual �ͷ��� ���� ������ �Լ��� �ִٸ� �Լ��� �����ϰ� ���ٸ� Air �ͷ�ó�� ����� ������
	// Torpedo ��ó�� �����ͷ��� ���� �����ϳ� �Ÿ����� ��� �ǰ� ��ź ��� ��ڸ� �߻�

	//if (IsValid(MouseTarget))
	//{
	//	FVector Pos = FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f);
	//	FVector TargetPos = FVector(MouseTarget->GetActorLocation().X, MouseTarget->GetActorLocation().Y, 0.0f);
	//	FVector Direction = (TargetPos - Pos).GetSafeNormal();
	//	float Angle = FRotationMatrix::MakeFromX(Direction).Rotator().Yaw;
	//	if (Angle < 0)
	//	{
	//		Angle += 360.0f;
	//	}

	//	MouseTargetDistance = FVector::Distance(Pos, TargetPos);

		//TargetSelectorComp->UpdateTargetInfo(ETurretType::Main, Angle);
		//TargetSelectorComp->SetTurretsTargetDistance(ETurretType::Main, MouseTargetDistance);
	//}
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
}


void UWHCTargetSelector::SetTurretsTargetDistance(ETurretType TurretType, float Dist)
{
	for (FTurretArray TArray : AllTurretArray)
	{
		if (TArray.TurretsType == TurretType)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				if (IsValid(Turret))
				{
					Turret->SetTargetDistance(Dist);
				}
			}
		}
	}
}

void UWHCTargetSelector::SetTurretsTargetAngle(ETurretType TurretType, float Angle)
{
	for (FTurretArray TArray : AllTurretArray)
	{
		if (TArray.TurretsType == TurretType)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				if (IsValid(Turret))
				{
					Turret->SetTargetAngle(Angle);
				}
			}
		}
	}
}

void UWHCTargetSelector::SetMainTurretTarget(AActor* Target)
{
	bIsTracingTarget = true;
	MainTarget = Target;
}

void UWHCTargetSelector::CommandTurretsFire(ETurretType TurretType)
{
	for (FTurretArray TArray : AllTurretArray)
	{
		if (TArray.TurretsType == TurretType)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				if (IsValid(Turret))
				{
					Turret->Fire();
				}
			}
		}
	}
}

void UWHCTargetSelector::ChangeAttackType(ETurretType TurretType, ETurretAttackType TurretAttackType)
{
}

void UWHCTargetSelector::UpdateTargetInfo(ETurretType TurretType, APawn* TargetPawn)
{
}

void UWHCTargetSelector::UpdateTargetInfo(ETurretType TurretType, FVector Point)
{
}

void UWHCTargetSelector::CalculateAirTurretRotation()
{
	ACTargetAngles.Empty();
	if (ACTargetArray.Num() != 0)
	{
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

