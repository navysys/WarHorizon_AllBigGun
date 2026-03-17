
#include "WarHorizon_AllBigGun/Aircraft/WHAircraftsBase.h"
#include "WarHorizon_AllBigGun/Aircraft/WHAircraft.h"
#include "WarHorizon_AllBigGun/Component/WHCAircraftsMovement.h"
#include "WarHorizon_AllBigGun/Controller/WHAircraftsAIController.h"
#include "WarHorizon_AllBigGun/Game/WHGameInstance.h"

// Sets default values
AWHAircraftsBase::AWHAircraftsBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AWHAircraftsAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bAlwaysRelevant = true;

	MaxSpeed = 1500.0f;

	DefaultSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMeshObject(TEXT("StaticMesh'/Game/Resource/Aircraft/Aircraft_Type0'"));
	if (DefaultMeshObject.Succeeded())
	{
		StaticMeshRef = DefaultMeshObject.Object;
	}
	AircraftMovementComp = CreateDefaultSubobject<UWHCAircraftsMovement>("AircraftMovementComp");
	AircraftMovementComp->MaxSpeed = 1500.0f;
	AircraftMovementComp->Acceleration = 3000.0f;
	AircraftMovementComp->Deceleration = 500.0f;
}

void AWHAircraftsBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWHAircraftsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveFront();
	SetAircraftFormations();
}

void AWHAircraftsBase::InitToDataTable(int Id)
{
	FName IDName = FName(FString::FromInt(Id));
	UDataTable* AircraftData = Cast<UWHGameInstance>(GetGameInstance())->GetAircraftDataTable();
	FAircraftDataTable* Table = AircraftData->FindRow<FAircraftDataTable>(IDName, "");

	if (Table)
	{
		ID = Id;
		Name = Table->AircraftName;
		AircraftsType = Table->AircraftsType;
		StaticMeshRef = Table->AircraftMesh;
		// 추후 모든 변수 테이블에서 가져오기

		
	}
	
}

void AWHAircraftsBase::MoveFront()
{
	if (IsValid(AircraftMovementComp))
	{
		AircraftMovementComp->AddInputVector(GetActorForwardVector());
	}
}

void AWHAircraftsBase::Turn(int Angle)
{
	if (IsValid(AircraftMovementComp))
	{
		AircraftMovementComp->Turn(Angle);
	}
}

void AWHAircraftsBase::IncreaseHeight(float MaxZ)
{
	if (IsValid(AircraftMovementComp))
	{
		AircraftMovementComp->IncreaseHeight(MaxZ);
	}
}

void AWHAircraftsBase::DecreaseHeight(float MinZ)
{
	if (IsValid(AircraftMovementComp))
	{
		AircraftMovementComp->DecreaseHeight(MinZ);
	}
}

void AWHAircraftsBase::GunAttack()
{
}

void AWHAircraftsBase::BombAttack()
{
}

void AWHAircraftsBase::ChangeMovePoint()
{

}

void AWHAircraftsBase::SpawnAircraft(FVector StartPos)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//임시 위치 조정
	AWHAircraft* SpawnedAircraft = GetWorld()->SpawnActor<AWHAircraft>(StartPos, FRotator::ZeroRotator, SpawnParams);
	SpawnedAircraft->StaticMeshComp->SetStaticMesh(StaticMeshRef);
	SpawnedAircraft->InitMaxSpeed = this->MaxSpeed;
	ArrayAircrafts.Emplace(SpawnedAircraft);
	AircraftsNum = ArrayAircrafts.Num();
	SetInitAircraftPosition();
}

uint8 AWHAircraftsBase::GetAircraftType()
{
	return (uint8)AircraftsType;
}

void AWHAircraftsBase::DestroyAircraft(int Index)
{
	if (ArrayAircrafts.Num() > Index)
	{
		ArrayAircrafts[Index]->Destroy();		// 차후에는 비행기가 불나면서 중력 낙하하도록하는 함수 호출
		ArrayAircrafts.RemoveAt(Index);
		AircraftsNum = ArrayAircrafts.Num();
		SetInitAircraftPosition();
	}
}

APawn* AWHAircraftsBase::GetMotherShip()
{
	return MotherShipPawn;
}

AActor* AWHAircraftsBase::GetTargetAircrafts()
{
	return Cast<AActor>(TargetAircrafts);
}

void AWHAircraftsBase::SetInitAircraftPosition()
{
	// 나중에 파일에서 위치값 받아오도록 변경
	InitAircraftPosition.Empty();

	if (AircraftsNum == 1)
	{
		InitAircraftPosition.Emplace(FVector::ZeroVector);
	}
	else if (AircraftsNum == 2)
	{
		InitAircraftPosition.Emplace(FVector::ZeroVector);
		InitAircraftPosition.Emplace(FVector(-100.0f, -1500.0f, 0.0f));
	}
	else if (AircraftsNum == 3)
	{
		InitAircraftPosition.Emplace(FVector::ZeroVector);
		InitAircraftPosition.Emplace(FVector(-100.0f, -1500.0f, 0.0f));
		InitAircraftPosition.Emplace(FVector(-100.0f, 1500.0f, 0.0f));
	}
	else if (AircraftsNum == 4)
	{
		InitAircraftPosition.Emplace(FVector::ZeroVector);
		InitAircraftPosition.Emplace(FVector(-100.0f, -1500.0f, 0.0f));
		InitAircraftPosition.Emplace(FVector(-100.0f, 1500.0f, 0.0f));
		InitAircraftPosition.Emplace(FVector(-300.0f, -3000.0f, 0.0f));
	}
}

void AWHAircraftsBase::SetAircraftFormations()
{
	FVector Location = GetActorLocation();
	FRotator Rotator = GetActorRotation();
	
	AircraftFormations.Empty();
	if (InitAircraftPosition.Num() > 0)
	{
		for (int i = 0; i < (int)AircraftsNum; i++)
		{
			AircraftFormations.Emplace(Location + Rotator.RotateVector(InitAircraftPosition[i]));
		}

		if (ArrayAircrafts.Num() > 0)
		{
			for (int i = 0; i < ArrayAircrafts.Num(); i++)
			{
				ArrayAircrafts[i]->CurrentPosition = AircraftFormations[i];
				ArrayAircrafts[i]->HeadDir = Rotator;
				ArrayAircrafts[i]->TargetHeight = Location.Z;
			}
		}
	}
}

void AWHAircraftsBase::SetTargetAircraft()
{

}

void AWHAircraftsBase::SetTargetBattleShip()
{

}

void AWHAircraftsBase::Fire()
{

}
