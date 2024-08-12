// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleShip/WHBattleShip.h"
#include "Game/WHGameSingleton.h"
#include "Controller/WHPlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Component/WHCBattleShipMovement.h"
#include "Component/WHCDetectEnemy.h"
#include "Component/WHCSkillHandler.h"
#include "Component/WHCTargetSelector.h"
#include "Turret/WHTurret.h"
#include "Aircraft/WHAircraftsBase.h"
#include "Enum/ETurretAttackType.h"
#include "Containers/Array.h"



// Sets default values
AWHBattleShip::AWHBattleShip()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Tags.Add(FName("BattleShip"));

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// 스켈레탈 매시
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetCollisionProfileName(TEXT("TeamABattleShipPreset"));
	RootComponent = SkeletalMeshComp;

	BattleShipMovementComp = CreateDefaultSubobject<UWHCBattleShipMovement>(TEXT("BattleShipMovementComp"));
	DetectEnemyComp = CreateDefaultSubobject<UWHCDetectEnemy>(TEXT("DetectEnemyComp"));
	SkillHandlerComp = CreateDefaultSubobject<UWHCSkillHandler>(TEXT("SkillHandlerComp"));
	TargetSelectorComp = CreateDefaultSubobject<UWHCTargetSelector>(TEXT("TargetSelectorComp"));
}

void AWHBattleShip::BeginPlay()
{
	Super::BeginPlay();

	//test
	SpawnAircrafts(0);
}

void AWHBattleShip::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	APawn* OwnerPawn = Cast<APawn>(GetInstigator());
	if (OwnerPawn != nullptr)
	{
		AWHPlayerController* OwnerController = Cast<AWHPlayerController>(OwnerPawn->GetController());
		if (OwnerController != nullptr)
		{
			BattleShipName = OwnerController->GetBattleShipData().BattleShipName;
		}
	}

	LoadDataTableToName(FName(BattleShipName));
	CreateTurretToMeshCompSocket(SkeletalMeshComp, FName(BattleShipName));

	if (BattleShipMovementComp != nullptr)
	{
		BattleShipMovementComp->InitBattleShipMovementComponent(InitMaxMoveSpeed, InitAcceleration, InitDeceleration, InitMaxRotationSpeed, InitRotationAcceleration, InitRotationAccelerationIncrease);
	}

	if (DetectEnemyComp != nullptr)
	{
		DetectEnemyComp->InitDetectEnemyComponent(TeamInt);
		DetectEnemyComp->SetDetectedBattleShips(&EnemyBattleShips);
		DetectEnemyComp->SetDetectedAircrafts(&EnemyAircrafts);
	}

	if (SkillHandlerComp != nullptr)
	{
		SkillHandlerComp->InitSkillHandlerComponent(SkillPtrQ, SkillPtrW, SkillPtrE, SkillPtrR);
	}

	if (TargetSelectorComp != nullptr)
	{
		TargetSelectorComp->InitTargetSelectorComponent(AllTurretArray, EnemyBattleShips, EnemyAircrafts);
	}
}

void AWHBattleShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMouseTarget)
	{
		CalculateAngleToSpinTurret();
	}
}


void AWHBattleShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWHBattleShip::UserFastFire()
{
	TargetSelectorComp->CommandTurretsFire(ETurretType::Main);
}

void AWHBattleShip::UserAttack()
{
	// 현재는 fastfire 와 같지만 이후에는 조준 완료 시에만 발사하고 조준이 안된 함포 회전하는 것 까지
	TargetSelectorComp->CommandTurretsFire(ETurretType::Main);
}

void AWHBattleShip::UserAttackCancel()
{
}

void AWHBattleShip::UserSpinTurrets(float Angle, float Distance)
{
	bIsMouseTarget = false;
	TargetSelectorComp->SetTurretsAttackType(ETurretType::Main, ETurretAttackType::AngleAttack);
	TargetSelectorComp->SetTurretsTarget(ETurretType::Main, Angle);
	TargetSelectorComp->SetTurretsTargetDistance(ETurretType::Main, Distance);
}

void AWHBattleShip::UserSpinTurretsToPawn(APawn* Target)
{
	TargetSelectorComp->SetTurretsAttackType(ETurretType::Main, ETurretAttackType::AngleAttack);
	bIsMouseTarget = true; // Tick 에서 타겟인 적을 적과의 거리를 지속적으로 계산

	if (IsValid(Target))
	{
		MouseTarget = Target;
		CalculateAngleToSpinTurret(); // Tick 호출 이전에도 계산
	}
}

void AWHBattleShip::UserSpinTurretsToHitPoint(FVector HitPoint)
{

}



void AWHBattleShip::UserSkill(char Key)
{

}

float AWHBattleShip::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}


/*			체력바 위젯 함수			*/




/*			인터페이스 오버라이드 함수			*/



void AWHBattleShip::CalculateRotationToHitPoint(FVector HitPoint)
{
	if (BattleShipMovementComp)
	{
		BattleShipMovementComp->CalculateAngle(HitPoint);
	}
}

void AWHBattleShip::IncreaseMoveSpeed()
{
	if (BattleShipMovementComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Increase call"));
		BattleShipMovementComp->IncreaseSpeedType();
	}
}

void AWHBattleShip::DecreaseMoveSpeed()
{
	if (BattleShipMovementComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Decrease call"));
		BattleShipMovementComp->DecreaseSpeedType();
	}
}

void AWHBattleShip::SpawnAircrafts(int Index)
{
	int Val = AircraftIDs[Index];

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	// MotherShip 초기화를 위해 사용
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (Val != 0)
	{
		AWHAircraftsBase* SpawnedAircrafts = GetWorld()->SpawnActor<AWHAircraftsBase>(GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		SpawnedAircrafts->InitToDataTable(AircraftIDs[Index]);
		AllAircraftsArray.Emplace(SpawnedAircrafts);

	}
}

void AWHBattleShip::LoadSingletonData()
{
	UE_LOG(LogTemp, Warning, TEXT("Load Singleton Data"));
	PlayerName = UWHGameSingleton::Get().GetPlayerName();
	BattleShipName = UWHGameSingleton::Get().GetBattleShipName();
	TeamInt = UWHGameSingleton::Get().GetTeamInt();
}

/*			데이터 관련 함수			*/
void AWHBattleShip::LoadDataTableToName(FName Name)
{
	UE_LOG(LogTemp, Warning, TEXT("Load DataTable Data"));
	UDataTable* BattleShipData = UWHGameSingleton::Get().GetBattleShipDataTable();
	FBattleShipDataTable* Table = BattleShipData->FindRow<FBattleShipDataTable>(Name, "");

	if (Table)
	{
		ID = Table->ID;
		BattleShipName = Table->Name;
		BattleShipType = Table->Type;
		BaseMesh = Table->BaseMesh;

		InitMaxHP = Table->MaxHP;
		InitMaxMP = Table->MaxMP;

		InitMaxMoveSpeed = Table->MoveSpeed;
		InitAcceleration = Table->Acceleration;
		InitDeceleration = Table->Acceleration / 2;
		InitMaxRotationSpeed = Table->RotationSpeed;
		InitRotationAcceleration = Table->RotationAcceleration;
		InitRotationAccelerationIncrease = Table->RotationAccelerationIncrease;

		SkillPtrQ = Table->SkillQ;
		SkillPtrW = Table->SkillW;
		SkillPtrE = Table->SkillE;
		SkillPtrR = Table->SkillR;

		AircraftIDs.Emplace(Table->AircraftsID);
		AircraftIDs.Emplace(Table->AircraftsID2);
		AircraftIDs.Emplace(Table->AircraftsID3);
		AircraftIDs.Emplace(Table->AircraftsID4);

		SkeletalMeshComp->SetSkeletalMesh(BaseMesh);
	}
}


// 매시 컴포넌트를 인자로 받아 소켓의 위치 방향을 받아 포탑을 생성
void AWHBattleShip::CreateTurretToMeshCompSocket(USkeletalMeshComponent* MeshComp, FName ShipName)
{
	FString BSName = ShipName.ToString();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Cast<APawn>(GetInstigator());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TArray<FName> MTSocketNames1;
	TArray<FName> MTSocketNames2;
	TArray<FName> DLSocketNames1;
	TArray<FName> DLSocketNames2;
	TArray<FName> STSocketNames1;
	TArray<FName> STSocketNames2;
	TArray<FName> ATSocketNames1;
	TArray<FName> ATSocketNames2;
	TArray<FName> DTSocketNames1;
	TArray<FName> DTSocketNames2;

	// 소켓 분류
	TArray<FName> SocketNames = SkeletalMeshComp->GetAllSocketNames();
	for (int i = 0; i < SocketNames.Num(); i++)
	{
		FName SocketName = SocketNames[i];
		FString StringName = SocketNames[i].ToString();

		if (StringName.Contains(TEXT("MT")))
		{
			if (StringName[3] == TCHAR('1'))
			{
				MTSocketNames1.AddUnique(SocketName);
			}
			else
			{
				MTSocketNames2.AddUnique(SocketName);
			}
		}
		else if (StringName.Contains(TEXT("ST")))
		{
			if (StringName[3] == TCHAR('1'))
			{
				STSocketNames1.AddUnique(SocketName);
			}
			else
			{
				STSocketNames2.AddUnique(SocketName);
			}
		}
		else if (StringName.Contains(TEXT("AT")))
		{
			if (StringName[3] == TCHAR('1'))
			{
				ATSocketNames1.AddUnique(SocketName);
			}
			else
			{
				ATSocketNames2.AddUnique(SocketName);
			}
		}
		else if (StringName.Contains(TEXT("DT")))
		{
			if (StringName[3] == TCHAR('1'))
			{
				DTSocketNames1.AddUnique(SocketName);
			}
			else
			{
				DTSocketNames2.AddUnique(SocketName);
			}
		}
		else if (StringName.Contains(TEXT("DL")))
		{
			if (StringName[3] == TCHAR('1'))
			{
				DLSocketNames1.AddUnique(SocketName);
			}
			else
			{
				DLSocketNames2.AddUnique(SocketName);
			}
		}
	}

	// 소켓에 포탑 생성 및 부착
	if (MTSocketNames1.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_MT_1");
		FTurretArray MainTurrets1;
		for (int i = 0; i < MTSocketNames1.Num(); i++)
		{
			const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(MTSocketNames1[i]);
			if (Sock != nullptr)
			{
				AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
				SpawnedTurret->SetupTurret(this, FName(TurretDataName));
				SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
				SpawnedTurret->SetFrontDirection(MTSocketNames1[i].ToString()[8]);

				MainTurrets1.Turrets.Emplace(SpawnedTurret);
			}
		}
		MainTurrets1.TurretsType = ETurretType::Main;
		AllTurretArray.Emplace(MainTurrets1);
	}
	if (MTSocketNames2.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_MT_2");
		FTurretArray MainTurrets2;
		for (int i = 0; i < MTSocketNames2.Num(); i++)
		{
			const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(MTSocketNames2[i]);
			if (Sock != nullptr)
			{
				AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
				SpawnedTurret->SetupTurret(this, FName(TurretDataName));
				SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
				SpawnedTurret->SetFrontDirection(MTSocketNames2[i].ToString()[8]);

				MainTurrets2.Turrets.Emplace(SpawnedTurret);
				
			}
		}
		MainTurrets2.TurretsType = ETurretType::Main;
		AllTurretArray.Emplace(MainTurrets2);
	}
	if (STSocketNames1.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_ST_1");
		FTurretArray SubTurrets1;
		for (int i = 0; i < STSocketNames1.Num(); i++)
		{
			const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(STSocketNames1[i]);
			if (Sock != nullptr)
			{
				AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
				SpawnedTurret->SetupTurret(this, FName(TurretDataName));
				SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
				SpawnedTurret->SetFrontDirection(STSocketNames1[i].ToString()[8]);

				SubTurrets1.Turrets.Emplace(SpawnedTurret);
			}
		}
		SubTurrets1.TurretsType = ETurretType::Sub;
		AllTurretArray.Emplace(SubTurrets1);
	}
	if (STSocketNames2.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_ST_2");
		FTurretArray SubTurrets2;
		for (int i = 0; i < STSocketNames2.Num(); i++)
		{
			const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(STSocketNames2[i]);
			if (Sock != nullptr)
			{
				AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
				SpawnedTurret->SetupTurret(this, FName(TurretDataName));
				SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
				SpawnedTurret->SetFrontDirection(STSocketNames2[i].ToString()[8]);

				SubTurrets2.Turrets.Emplace(SpawnedTurret);
			}
		}
		SubTurrets2.TurretsType = ETurretType::Sub;
		AllTurretArray.Emplace(SubTurrets2);
	}
	if (ATSocketNames1.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_AT_1");
		FTurretArray AirTurrets1;
		for (int i = 0; i < ATSocketNames1.Num(); i++)
		{
			const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(ATSocketNames1[i]);
			if (Sock != nullptr)
			{
				AWHTurretBase* SpawnedTurret = GetWorld()->SpawnActor<AWHTurretBase>(Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
				SpawnedTurret->SetupTurret(this, FName(TurretDataName));
				SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
				SpawnedTurret->SetFrontDirection(ATSocketNames1[i].ToString()[8]);

				AirTurrets1.Turrets.Emplace(SpawnedTurret);
			}
		}
		AirTurrets1.TurretsType = ETurretType::Air;
		AllTurretArray.Emplace(AirTurrets1);
	}
	if (ATSocketNames2.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_AT_2");
		FTurretArray AirTurrets2;
		for (int i = 0; i < ATSocketNames2.Num(); i++)
		{
			const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(ATSocketNames2[i]);
			if (Sock != nullptr)
			{
				AWHTurretBase* SpawnedTurret = GetWorld()->SpawnActor<AWHTurretBase>(Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
				SpawnedTurret->SetupTurret(this, FName(TurretDataName));
				SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
				SpawnedTurret->SetFrontDirection(ATSocketNames2[i].ToString()[8]);

				AirTurrets2.Turrets.Emplace(SpawnedTurret);
			}
		}
		AirTurrets2.TurretsType = ETurretType::Air;
		AllTurretArray.Emplace(AirTurrets2);
	}
	if (DTSocketNames1.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_DT_1");
		FTurretArray DualTurrets1;
		for (int i = 0; i < DTSocketNames1.Num(); i++)
		{
			const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(DTSocketNames1[i]);
			if (Sock != nullptr)
			{
				AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
				SpawnedTurret->SetupTurret(this, FName(TurretDataName));
				SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
				SpawnedTurret->SetFrontDirection(DTSocketNames1[i].ToString()[8]);

				DualTurrets1.Turrets.Emplace(SpawnedTurret);
			}
		}
		DualTurrets1.TurretsType = ETurretType::DualPurpose;
		AllTurretArray.Emplace(DualTurrets1);
	}
	if (DTSocketNames2.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_DT_2");
		FTurretArray DualTurrets2;
		for (int i = 0; i < DTSocketNames2.Num(); i++)
		{
			const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(DTSocketNames2[i]);
			if (Sock != nullptr)
			{
				AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
				SpawnedTurret->SetupTurret(this, FName(TurretDataName));
				SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
				SpawnedTurret->SetFrontDirection(DTSocketNames2[i].ToString()[8]);

				DualTurrets2.Turrets.Emplace(SpawnedTurret);
			}
		}
		DualTurrets2.TurretsType = ETurretType::DualPurpose;
		AllTurretArray.Emplace(DualTurrets2);
	}
	if (DLSocketNames1.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_DL_1");
		FTurretArray TorpedoLaunchers1;
		for (int i = 0; i < DLSocketNames1.Num(); i++)
		{
			const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(DLSocketNames1[i]);
			if (Sock != nullptr)
			{
				AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
				SpawnedTurret->SetupTurret(this, FName(TurretDataName));
				SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
				SpawnedTurret->SetFrontDirection(DLSocketNames1[i].ToString()[8]);

				TorpedoLaunchers1.Turrets.Emplace(SpawnedTurret);
			}
		}
		TorpedoLaunchers1.TurretsType = ETurretType::Torpedo;
		AllTurretArray.Emplace(TorpedoLaunchers1);
	}
	if (DLSocketNames2.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_DL_2");
		FTurretArray TorpedoLaunchers2;
		for (int i = 0; i < DLSocketNames2.Num(); i++)
		{
			const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(DLSocketNames2[i]);
			if (Sock != nullptr)
			{
				AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
				SpawnedTurret->SetupTurret(this, FName(TurretDataName));
				SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
				SpawnedTurret->SetFrontDirection(DLSocketNames2[i].ToString()[8]);

				TorpedoLaunchers2.Turrets.Emplace(SpawnedTurret);
			}
		}
		TorpedoLaunchers2.TurretsType = ETurretType::Torpedo;
		AllTurretArray.Emplace(TorpedoLaunchers2);
	}
}



void AWHBattleShip::CalculateAngleToSpinTurret()
{
	if (IsValid(MouseTarget))
	{
		FVector Pos = FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f);
		FVector TargetPos = FVector(MouseTarget->GetActorLocation().X, MouseTarget->GetActorLocation().Y, 0.0f);
		FVector Direction = (TargetPos - Pos).GetSafeNormal();
		float Angle = FRotationMatrix::MakeFromX(Direction).Rotator().Yaw;
		if (Angle < 0)
		{
			Angle += 360.0f;
		}

		MouseTargetDistance = FVector::Distance(Pos, TargetPos);

		TargetSelectorComp->SetTurretsTarget(ETurretType::Main, Angle);
		TargetSelectorComp->SetTurretsTargetDistance(ETurretType::Main, MouseTargetDistance);
	}
}
