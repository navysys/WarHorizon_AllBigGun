// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleShip/WHBattleShipBase.h"
#include "Game/WHGameInstance.h"
#include "Controller/WHPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Component/WHCBattleShipMovement.h"
#include "Component/WHCDetectEnemy.h"
#include "Component/WHCTargetSelector.h"
#include "Component/WHCCameraBodyComponent.h"
#include "Turret/WHTurretBase.h"
#include "Aircraft/WHAircraftsBase.h"
#include "Enum/ETurretAttackType.h"
#include "Containers/Array.h"
#include "Skill/WHSkillBase.h"
#include "Skill/WHSkillYamatoQ.h"


AWHBattleShipBase::AWHBattleShipBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Tags.Add(FName("BattleShip"));

	bAlwaysRelevant = true;

	// 스켈레탈 매시
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComp"));
	BoxComp->SetBoxExtent(BoxSize);
	BoxComp->SetNotifyRigidBodyCollision(true);
	BoxComp->SetCollisionProfileName(TEXT("TeamABattleShipPreset"));
	RootComponent = BoxComp;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(BoxComp);
	// 스켈레탈 매시의 콜리전 프로파일은 나중에 수정

	SmokestackComp = CreateDefaultSubobject<UNiagaraComponent>("SmokeStackNiagara");
	SmokestackComp->SetupAttachment(SkeletalMeshComp, TEXT("Smokestack"));
	if (Smokestack)
	{
		SmokestackComp->SetAsset(Smokestack);
	}

	CameraBodyComp = CreateDefaultSubobject<UWHCCameraBodyComponent>(TEXT("CameraBody"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetRelativeRotation(FRotator(-50.0f, 90.0f, 0.0f));
	SpringArmComp->TargetArmLength = 50000.0f;
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritYaw = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->SetupAttachment(CameraBodyComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);
	
	BattleShipMovementComp = CreateDefaultSubobject<UWHCBattleShipMovement>(TEXT("BattleShipMovementComp"));
	DetectEnemyComp = CreateDefaultSubobject<UWHCDetectEnemy>(TEXT("DetectEnemyComp"));
	TargetSelectorComp = CreateDefaultSubobject<UWHCTargetSelector>(TEXT("TargetSelectorComp"));

	SkillQ = CreateDefaultSubobject<UWHSkillYamatoQ>(TEXT("SkillQ"));
	SkillQ->Init(this);
}

void AWHBattleShipBase::BeginPlay()
{
	Super::BeginPlay();

	SkillQ->Effect();
	//test
	//SpawnAircrafts(0);
}

void AWHBattleShipBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SmokestackComp->SetIntParameter(TEXT("SpawnRate"), 0);

	LoadDataTableToName(FName(BattleShipName));
	CreateTurretToMeshCompSocket(SkeletalMeshComp);

	if (IsValid(BattleShipMovementComp))
	{
		BattleShipMovementComp->InitBattleShipMovementComponent(InitMaxMoveSpeed, InitAcceleration, InitDeceleration, InitMaxRotationSpeed, InitRotationAcceleration, InitRotationAccelerationIncrease);
	}

	if (IsValid(DetectEnemyComp))
	{
		DetectEnemyComp->InitDetectEnemyComponent(TeamInt);
		DetectEnemyComp->SetDetectedBattleShips(&EnemyBattleShips);
		DetectEnemyComp->SetDetectedAircrafts(&EnemyAircrafts);
	}


	if (IsValid(TargetSelectorComp))
	{
		TargetSelectorComp->InitTargetSelectorComponent(AllTurretArray, &EnemyBattleShips, EnemyAircrafts);
	}
}

void AWHBattleShipBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	uint8 DirectionCharIndex = 8;

	// 소켓에 포탑 생성 및 부착
	if (SocketNames.Num() > 0)
	{
		FTurretArray MainTurrets;
		FTurretArray SubTurrets;
		FTurretArray AirTurrets;
		FTurretArray DualTurrets;
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
						SpawnedTurret->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, Sock->SocketName);
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[DirectionCharIndex]);

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
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[DirectionCharIndex]);

						SubTurrets.Turrets.Emplace(SpawnedTurret);
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
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[DirectionCharIndex]);

						SubTurrets.Turrets.Emplace(SpawnedTurret);
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
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[DirectionCharIndex]);

						AirTurrets.Turrets.Emplace(SpawnedTurret);
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
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[DirectionCharIndex]);

						AirTurrets.Turrets.Emplace(SpawnedTurret);
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
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[DirectionCharIndex]);

						DualTurrets.Turrets.Emplace(SpawnedTurret);
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
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[DirectionCharIndex]);

						DualTurrets.Turrets.Emplace(SpawnedTurret);
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
						SpawnedTurret->SetFrontDirection(SocketNames[i].ToString()[DirectionCharIndex]);

						TorpedoLaunchers.Turrets.Emplace(SpawnedTurret);
					}
				}
			}	
		}
		MainTurrets.TurretsType = ETurretType::Main;
		SubTurrets.TurretsType = ETurretType::Sub;
		AirTurrets.TurretsType = ETurretType::Air;
		DualTurrets.TurretsType = ETurretType::DualPurpose;
		TorpedoLaunchers.TurretsType = ETurretType::Torpedo;

		AllTurretArray.Emplace(MainTurrets);
		AllTurretArray.Emplace(SubTurrets);
		AllTurretArray.Emplace(AirTurrets);
		AllTurretArray.Emplace(DualTurrets);
		AllTurretArray.Emplace(TorpedoLaunchers);
	}
}

void AWHBattleShipBase::RapidAttack()
{
	if (IsValid(TargetSelectorComp))
	{
		TargetSelectorComp->CommandTurretsFire(ETurretType::Main);
	}
}

void AWHBattleShipBase::NormalAttack()
{
	// 현재는 fastfire 와 같지만 이후에는 조준 완료 시에만 발사하고 조준이 안된 함포 회전하는 것 까지
	UE_LOG(LogTemp, Warning, TEXT("Normal Attack Called"));
	if (IsValid(TargetSelectorComp))
	{
		TargetSelectorComp->CommandTurretsFire(ETurretType::Main);
	}
}


void AWHBattleShipBase::SpinTurrets(AActor* Target)
{
	if (IsValid(TargetSelectorComp))
	{
		TargetSelectorComp->SetMainTurretTarget(Target);
	}
}

void AWHBattleShipBase::SpinTurrets(FVector HitPoint)
{
	if (IsValid(TargetSelectorComp))
	{
		TargetSelectorComp->SetMainTurretPoint(HitPoint);
	}
}

void AWHBattleShipBase::SpinBattleShip(FVector HitPoint)
{
	if (IsValid(BattleShipMovementComp))
	{
		BattleShipMovementComp->CalculateAngle(HitPoint);
	}
}

void AWHBattleShipBase::IncreaseMoveSpeed()
{
	if (IsValid(BattleShipMovementComp))
	{
		int32 Value = 0;
		BattleShipMovementComp->IncreaseSpeedType();
		if (BattleShipMovementComp->SpeedType == ESpeedType::Max)
		{
			SmokestackComp->SetIntParameter(TEXT("SpawnRate"), 120);
		}
		else if (BattleShipMovementComp->SpeedType == ESpeedType::ThreeQuater)
		{
			SmokestackComp->SetIntParameter(TEXT("SpawnRate"), 80);
		}
		else if (BattleShipMovementComp->SpeedType == ESpeedType::OneHalf)
		{
			SmokestackComp->SetIntParameter(TEXT("SpawnRate"), 60);
		}
		else if (BattleShipMovementComp->SpeedType == ESpeedType::OneQuater)
		{
			SmokestackComp->SetIntParameter(TEXT("SpawnRate"), 30);
		}
		else if (BattleShipMovementComp->SpeedType == ESpeedType::Stop)
		{
			SmokestackComp->SetIntParameter(TEXT("SpawnRate"), 0);
		}
		else if (BattleShipMovementComp->SpeedType == ESpeedType::Back)
		{
			SmokestackComp->SetIntParameter(TEXT("SpawnRate"), 40);
		}
	}
}

void AWHBattleShipBase::DecreaseMoveSpeed()
{
	if (IsValid(BattleShipMovementComp))
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

UObject* AWHBattleShipBase::GetSkill(char Button)
{
	if (Button == 'Q')
	{
		return SkillQ;
	}

	return nullptr;
}

