
#include "Component/WHCTargetSelector.h"
#include "Turret/WHTurretBase.h"


UWHCTargetSelector::UWHCTargetSelector()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWHCTargetSelector::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = GetOwner();
}

void UWHCTargetSelector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Main 터렛은 유저 컨트롤러의 인풋을 받아 Point 의 좌표를 조준하거나 Target 을 트래킹하거나 공격할 수 있어야 함
	// 일정 시간마다 SUB , AIR 터렛은 배열에 있는 적 중에 공격할 수 있는 것을 자동 공격해야함
	// Dual 터렛은 공격 가능한 함선이 있다면 함선을 공격하고 없다면 Air 터렛처럼 대공을 수행함
	// Torpedo 런처는 메인터렛과 같이 동작하나 거리값은 없어도 되고 포탄 대신 어뢰를 발사

	SetSubTurretDataToTargetArray();
	for (AWHTurretBase* Turret : ForwardSTs)
	{
		if (STLDatas.Num())
		{
			Turret->SetTargetData(&STFDatas);
		}
	}
	for (AWHTurretBase* Turret : BackSTs)
	{
		if (STLDatas.Num())
		{
			Turret->SetTargetData(&STBDatas);
		}
	}

	if (bIsTracingTarget)
	{
		if (IsValid(MTTarget))
		{
			// 함선과 타겟 사이의 각도와 거리 계산 후 전달
			FTargetData Data = CalculateOwnerToPointData(MTTarget->GetActorLocation());

			for (AWHTurretBase* Turret : MainTurrets)
			{
				Turret->SetTargetData(Data);
			}

		}
	}
}

void UWHCTargetSelector::InitTargetSelectorComponent(const TArray<FTurretArray> AllArray,TArray<APawn*>* BattleShips, TArray<APawn*> Aircrafts)
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
	MTTarget = Target;
}

void UWHCTargetSelector::SetMainTurretPoint(FVector Point)
{
	bIsTracingTarget = false;
	FTargetData Data = CalculateOwnerToPointData(Point);
	for (AWHTurretBase* Turret : MainTurrets)
	{
		Turret->SetTargetData(Data);
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
				if (IsValid(Turret))
				{
					Turret->Fire();
				}
			}
		}
	}
}

void UWHCTargetSelector::SetAirTurretDataToTargetArray()
{
	ACTargetDatas.Empty();
	if (ACTargetArray.Num() != 0)
	{
		for (APawn* AC : ACTargetArray)
		{
			FTargetData Angle = CalculateOwnerToPointData(AC->GetActorLocation());
			ACTargetDatas.Emplace(Angle);
		}
		
		if (ACTargetDatas.Num() != 0)
		{
			bool L = true;
			bool R = true;
			bool F = true;
			bool B = true;
			for (int i = 0; i < ACTargetDatas.Num(); i++)
			{
				if (ACTargetDatas[i].Angle <= -30.0f && ACTargetDatas[i].Angle >= -150.0f)
				{
					if (L)
					{
						ATLData = ACTargetDatas[i];
						L = false;
					}
				}
				else if (ACTargetDatas[i].Angle >= 30.0f && ACTargetDatas[i].Angle <= 150.0f)
				{
					if (R)
					{
						ATRData = ACTargetDatas[i];
						R = false;
					}
				}
				else if (ACTargetDatas[i].Angle > -30.0f && ACTargetDatas[i].Angle < 30.0f)
				{
					if (F)
					{
						ATFData = ACTargetDatas[i];
						F = false;
					}
				}
				else if (ACTargetDatas[i].Angle > 150.0f && ACTargetDatas[i].Angle < -150.0f)
				{
					if (B)
					{
						ATBData = ACTargetDatas[i];
						B = false;
					}
				}
			}
		}
	}
}

void UWHCTargetSelector::SetSubTurretDataToTargetArray()
{
	BSTargetDatas.Empty();
	if (BSTargetArray->Num() != 0)
	{
		
		for (APawn* BS : *BSTargetArray)
		{
			FTargetData Data = CalculateOwnerToPointData(BS->GetActorLocation());
			BSTargetDatas.Emplace(Data);
		}

		if (BSTargetDatas.Num() != 0)
		{
			for (int i = 0; i < BSTargetDatas.Num(); i++)
			{
				if (BSTargetDatas[i].Angle <= -30.0f && BSTargetDatas[i].Angle >= -150.0f)
				{
					STLDatas.Emplace(BSTargetDatas[i]);
				}
				if (BSTargetDatas[i].Angle >= 30.0f && BSTargetDatas[i].Angle <= 150.0f)
				{
					STRDatas.Emplace(BSTargetDatas[i]);
				}
				if (BSTargetDatas[i].Angle > -120.0f && BSTargetDatas[i].Angle < 120.0f)
				{
					STFDatas.Emplace(BSTargetDatas[i]);
				}
				if (BSTargetDatas[i].Angle > 60.0f && BSTargetDatas[i].Angle < -60.0f)
				{
					STBDatas.Emplace(BSTargetDatas[i]);
				}
			}
		}
	}
}

FTargetData UWHCTargetSelector::CalculateOwnerToPointData(FVector Point)
{
	FVector Pos = Owner->GetActorLocation();
	FVector TPos = Point;

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

	return FTargetData(Angle, Dist);
}
