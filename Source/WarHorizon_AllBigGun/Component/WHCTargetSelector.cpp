
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

	// Main 터렛은 유저 컨트롤러의 인풋을 받아 Point 의 좌표를 조준하거나 Target 을 트래킹하거나 공격할 수 있어야 함
	// 일정 시간마다 SUB , AIR 터렛은 배열에 있는 적 중에 공격할 수 있는 것을 자동 공격해야함
	// Dual 터렛은 공격 가능한 함선이 있다면 함선을 공격하고 없다면 Air 터렛처럼 대공을 수행함
	// Torpedo 런처는 메인터렛과 같이 동작하나 거리값은 없어도 되고 포탄 대신 어뢰를 발사

	if (bIsTracingTarget)
	{
		if (IsValid(MainTarget))
		{
			// 함선과 타겟 사이의 각도와 거리 계산 후 전달
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
