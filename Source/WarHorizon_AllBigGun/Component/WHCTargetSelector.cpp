
#include "Component/WHCTargetSelector.h"
#include "Turret/WHTurretBase.h"



UWHCTargetSelector::UWHCTargetSelector()
{
	PrimaryComponentTick.bCanEverTick = true;

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

	// Main �ͷ��� ���� ��Ʈ�ѷ��� ��ǲ�� �޾� Point �� ��ǥ�� �����ϰų� Target �� Ʈ��ŷ�ϰų� ������ �� �־�� ��
	// ���� �ð����� SUB , AIR �ͷ��� �迭�� �ִ� �� �߿� ������ �� �ִ� ���� �ڵ� �����ؾ���
	// Dual �ͷ��� ���� ������ �Լ��� �ִٸ� �Լ��� �����ϰ� ���ٸ� Air �ͷ�ó�� ����� ������
	// Torpedo ��ó�� �����ͷ��� ���� �����ϳ� �Ÿ����� ��� �ǰ� ��ź ��� ��ڸ� �߻�

	if (bIsTracingTarget)
	{
		if (IsValid(MainTarget))
		{
			// �Լ��� Ÿ�� ������ ������ �Ÿ� ��� �� ����
			FVector Pos = Owner->GetActorLocation();
			FVector TPos = MainTarget->GetActorLocation();

			float Dist = FVector::Dist2D(Pos, TPos);
			
			FVector Dir = (TPos - Pos).GetSafeNormal2D();
			float Yaw = FRotationMatrix::MakeFromX(Dir).Rotator().Yaw;
			float Angle = round(Yaw - Owner->GetActorRotation().Yaw);
			if (Angle > 180.0f)
			{
				Angle -= 360.0f;
			}
			else if (Angle < -180.0f)
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

	}

}

void UWHCTargetSelector::InitTargetSelectorComponent(const TArray<FTurretArray> AllArray, TArray<APawn*> BattleShips, TArray<APawn*> Aircrafts)
{
	AllTurretArray = AllArray;
	BSTargetArray = BattleShips;
	ACTargetArray = Aircrafts;

	for (FTurretArray TArray : AllTurretArray)
	{
		if (TArray.TurretsType == ETurretType::Main)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				MainTurrets.Emplace(Turret);
			}
		}
		else if (TArray.TurretsType == ETurretType::Sub)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				char Dir = Turret->GetFrontDirection();
				if (Dir == 'L')
				{
					LeftSTs.Emplace(Turret);
				}
				else if (Dir == 'R')
				{
					RightSTs.Emplace(Turret);
				}
				else if (Dir == 'F')
				{
					ForwardSTs.Emplace(Turret);
				}
				else if (Dir == 'B')
				{
					BackSTs.Emplace(Turret);
				}
			}
		}
		else if (TArray.TurretsType == ETurretType::Air)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				char Dir = Turret->GetFrontDirection();
				if (Dir == 'L')
				{
					LeftATs.Emplace(Turret);
				}
				else if (Dir == 'R')
				{
					RightATs.Emplace(Turret);
				}
				else if (Dir == 'F')
				{
					ForwardATs.Emplace(Turret);
				}
				else if (Dir == 'B')
				{
					BackATs.Emplace(Turret);
				}
			}
		}
		else if (TArray.TurretsType == ETurretType::DualPurpose)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				char Dir = Turret->GetFrontDirection();
				if (Dir == 'L')
				{
					LeftDTs.Emplace(Turret);
				}
				else if (Dir == 'R')
				{
					RightDTs.Emplace(Turret);
				}
				else if (Dir == 'F')
				{
					ForwardDTs.Emplace(Turret);
				}
				else if (Dir == 'B')
				{
					BackDTs.Emplace(Turret);
				}
			}
		}
		else if (TArray.TurretsType == ETurretType::Torpedo)
		{
			for (AWHTurretBase* Turret : TArray.Turrets)
			{
				MainTurrets.Emplace(Turret);
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
