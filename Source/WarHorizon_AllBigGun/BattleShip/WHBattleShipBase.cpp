// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleShip/WHBattleShipBase.h"
#include "Game/WHGameInstance.h"
#include "Controller/WHPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Component/WHCBattleShipMovement.h"
#include "Component/WHCDetectEnemy.h"
#include "Component/WHCSkillHandler.h"
#include "Component/WHCTargetSelector.h"
#include "Turret/WHTurret.h"
#include "Aircraft/WHAircraftsBase.h"
#include "Enum/ETurretAttackType.h"
#include "Containers/Array.h"
#include "Skill/WHSkillBase.h"

// Sets default values
AWHBattleShipBase::AWHBattleShipBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Tags.Add(FName("BattleShip"));

	// 스켈레탈 매시
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootComp"));
	RootComponent = CapsuleComp;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	//SkeletalMeshComp->SetCollisionProfileName(TEXT("TeamABattleShipPreset"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetRelativeRotation(FRotator(-50.0f, 90.0f, 0.0f));
	SpringArmComp->TargetArmLength = 50000.0f;
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritYaw = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);
	
	BattleShipMovementComp = CreateDefaultSubobject<UWHCBattleShipMovement>(TEXT("BattleShipMovementComp"));
	DetectEnemyComp = CreateDefaultSubobject<UWHCDetectEnemy>(TEXT("DetectEnemyComp"));
	SkillHandlerComp = CreateDefaultSubobject<UWHCSkillHandler>(TEXT("SkillHandlerComp"));
	TargetSelectorComp = CreateDefaultSubobject<UWHCTargetSelector>(TEXT("TargetSelectorComp"));
}

// Called when the game starts or when spawned
void AWHBattleShipBase::BeginPlay()
{
	Super::BeginPlay();
	
	//test
	SpawnAircrafts(0);
}

void AWHBattleShipBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	LoadDataTableToName(FName(BattleShipName));
	CreateTurretToMeshCompSocket(SkeletalMeshComp);

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

// Called every frame
void AWHBattleShipBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMouseTarget)
	{
		CalculateAngleToSpinTurret();
	}
}

float AWHBattleShipBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}


void AWHBattleShipBase::LoadDataTableToName(FName Name)
{
	UWHGameInstance* GI = Cast<UWHGameInstance>(GetGameInstance());
	if (GI)
	{
		UDataTable* BattleShipData = GI->GetBattleShipDataTable();
		if (BattleShipData)
		{
			FBattleShipDataTable* Table = BattleShipData->FindRow<FBattleShipDataTable>(Name, "");

			if (Table)
			{
				ID = Table->ID;
				BattleShipName = Table->Name;
				BattleShipType = Table->Type;

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
			}
		}
	}
}

void AWHBattleShipBase::CreateTurretToMeshCompSocket(USkeletalMeshComponent* MeshComp)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TArray<FName> SocketNames = SkeletalMeshComp->GetAllSocketNames();

	// 소켓에 포탑 생성 및 부착
	if (SocketNames.Num() > 0)
	{
		FTurretArray MainTurrets;
		FTurretArray SubTurrets1;
		FTurretArray SubTurrets2;
		FTurretArray AirTurrets1;
		FTurretArray AirTurrets2;
		FTurretArray DualTurrets1;
		FTurretArray DualTurrets2;
		FTurretArray TorpedoLaunchers;

		for (int i = 0; i < SocketNames.Num(); i++)
		{
			FName SocketName = SocketNames[i];
			FString StringName = SocketNames[i].ToString();
			if (StringName.Contains(TEXT("MT")))
			{
				if (MainTurret != nullptr)
				{
					const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(SocketNames[i]);
					if (Sock != nullptr)
					{
						AWHTurretBase* SpawnedTurret = GetWorld()->SpawnActor<AWHTurretBase>(MainTurret, Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
						SpawnedTurret->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[8]);

						MainTurrets.Turrets.Emplace(SpawnedTurret);
					}
				}
			}
			else if (StringName.Contains(TEXT("ST_1")))
			{
				if (SubTurret1 != nullptr)
				{
					const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(SocketNames[i]);
					if (Sock != nullptr)
					{
						AWHTurretBase* SpawnedTurret = GetWorld()->SpawnActor<AWHTurretBase>(SubTurret1, Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
						SpawnedTurret->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[8]);

						SubTurrets1.Turrets.Emplace(SpawnedTurret);
					}
				}
			}
			else if (StringName.Contains(TEXT("ST_2")))
			{
				if (SubTurret2 != nullptr)
				{
					const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(SocketNames[i]);
					if (Sock != nullptr)
					{
						AWHTurretBase* SpawnedTurret = GetWorld()->SpawnActor<AWHTurretBase>(SubTurret2, Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
						SpawnedTurret->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[8]);

						SubTurrets2.Turrets.Emplace(SpawnedTurret);
					}
				}
			}
			else if (StringName.Contains(TEXT("AT_1")))
			{
				if (AirTurret1 != nullptr)
				{
					const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(SocketNames[i]);
					if (Sock != nullptr)
					{
						AWHTurretBase* SpawnedTurret = GetWorld()->SpawnActor<AWHTurretBase>(AirTurret1, Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
						SpawnedTurret->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[8]);

						AirTurrets1.Turrets.Emplace(SpawnedTurret);
					}
				}
			}
			else if (StringName.Contains(TEXT("AT_2")))
			{
				if (AirTurret2 != nullptr)
				{
					const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(SocketNames[i]);
					if (Sock != nullptr)
					{
						AWHTurretBase* SpawnedTurret = GetWorld()->SpawnActor<AWHTurretBase>(AirTurret2, Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
						SpawnedTurret->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[8]);

						AirTurrets2.Turrets.Emplace(SpawnedTurret);
					}
				}
			}
			else if (StringName.Contains(TEXT("DT_1")))
			{
				if (DualTurret1 != nullptr)
				{
					const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(SocketNames[i]);
					if (Sock != nullptr)
					{
						AWHTurretBase* SpawnedTurret = GetWorld()->SpawnActor<AWHTurretBase>(DualTurret1, Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
						SpawnedTurret->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[8]);

						DualTurrets1.Turrets.Emplace(SpawnedTurret);
					}
				}
			}
			else if (StringName.Contains(TEXT("DT_2")))
			{
				if (DualTurret2 != nullptr)
				{
					const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(SocketNames[i]);
					if (Sock != nullptr)
					{
						AWHTurretBase* SpawnedTurret = GetWorld()->SpawnActor<AWHTurretBase>(DualTurret2, Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
						SpawnedTurret->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[8]);

						DualTurrets2.Turrets.Emplace(SpawnedTurret);
					}
				}
			}
			else if (StringName.Contains(TEXT("DL")))
			{
				if (TorpedoLauncher != nullptr)
				{
					const USkeletalMeshSocket* Sock = SkeletalMeshComp->GetSocketByName(SocketNames[i]);
					if (Sock != nullptr)
					{
						AWHTurretBase* SpawnedTurret = GetWorld()->SpawnActor<AWHTurretBase>(TorpedoLauncher, Sock->GetSocketLocation(SkeletalMeshComp), FRotator::ZeroRotator, SpawnParams);
						SpawnedTurret->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Sock->SocketName);
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[8]);

						TorpedoLaunchers.Turrets.Emplace(SpawnedTurret);
					}
				}
			}	
		}
		MainTurrets.TurretsType = ETurretType::Main;
		SubTurrets1.TurretsType = ETurretType::Sub;
		SubTurrets2.TurretsType = ETurretType::Sub;
		AirTurrets1.TurretsType = ETurretType::Air;
		AirTurrets2.TurretsType = ETurretType::Air;
		DualTurrets1.TurretsType = ETurretType::DualPurpose;
		DualTurrets2.TurretsType = ETurretType::DualPurpose;
		TorpedoLaunchers.TurretsType = ETurretType::Torpedo;

		AllTurretArray.Emplace(MainTurrets);
		AllTurretArray.Emplace(SubTurrets1);
		AllTurretArray.Emplace(SubTurrets2);
		AllTurretArray.Emplace(AirTurrets1);
		AllTurretArray.Emplace(AirTurrets2);
		AllTurretArray.Emplace(DualTurrets1);
		AllTurretArray.Emplace(DualTurrets2);
		AllTurretArray.Emplace(TorpedoLaunchers);
	}
}

void AWHBattleShipBase::CalculateAngleToSpinTurret()
{
	// 클라이언트에서만 실행할 함수
// 타겟 셀렉터 안으로 넣어버리는 것 고려중
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

void AWHBattleShipBase::RapidAttack()
{
	TargetSelectorComp->CommandTurretsFire(ETurretType::Main);
}

void AWHBattleShipBase::NormalAttack()
{
	// 현재는 fastfire 와 같지만 이후에는 조준 완료 시에만 발사하고 조준이 안된 함포 회전하는 것 까지
	TargetSelectorComp->CommandTurretsFire(ETurretType::Main);
}

void AWHBattleShipBase::SpinTurrets(float Angle, float Distance)
{
	bIsMouseTarget = false;
	TargetSelectorComp->SetTurretsAttackType(ETurretType::Main, ETurretAttackType::AngleAttack);
	TargetSelectorComp->SetTurretsTarget(ETurretType::Main, Angle);
	TargetSelectorComp->SetTurretsTargetDistance(ETurretType::Main, Distance);
}

void AWHBattleShipBase::SpinTurrets(APawn* Target)
{
	TargetSelectorComp->SetTurretsAttackType(ETurretType::Main, ETurretAttackType::AngleAttack);
	bIsMouseTarget = true; // Tick 에서 타겟인 적을 적과의 거리를 지속적으로 계산

	if (IsValid(Target))
	{
		MouseTarget = Target;
		CalculateAngleToSpinTurret(); // Tick 호출 이전에도 계산
	}
}

void AWHBattleShipBase::SpinTurrets(FVector HitPoint)
{

}

void AWHBattleShipBase::UseSkill(char Key)
{

}

void AWHBattleShipBase::SpinBattleShip(FVector HitPoint)
{
	if (BattleShipMovementComp)
	{
		BattleShipMovementComp->CalculateAngle(HitPoint);
	}
}

void AWHBattleShipBase::IncreaseMoveSpeed()
{
	if (BattleShipMovementComp)
	{
		BattleShipMovementComp->IncreaseSpeedType();
	}
}

void AWHBattleShipBase::DecreaseMoveSpeed()
{
	if (BattleShipMovementComp)
	{
		BattleShipMovementComp->DecreaseSpeedType();
	}
}

void AWHBattleShipBase::SpawnAircrafts(int Index)
{
	int Val = AircraftIDs[Index];

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	// MotherShip 초기화를 위해 사용
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (Val != 0)
	{
		// 소켓 위치에서 스폰되게 이후 변경해야 함
		AWHAircraftsBase* SpawnedAircrafts = GetWorld()->SpawnActor<AWHAircraftsBase>(GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		SpawnedAircrafts->InitToDataTable(AircraftIDs[Index]);
		AllAircraftsArray.Emplace(SpawnedAircrafts);

	}
}

