
#include "Aircraft/WHAircraftsBase.h"
#include "Aircraft/WHAircraft.h"
#include "Component/WHCAircraftsMovement.h"
#include "Controller/WHAircraftsAIController.h"
#include "Game/WHGameInstance.h"

// Sets default values
AWHAircraftsBase::AWHAircraftsBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AWHAircraftsAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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
		// ���� ��� ���� ���̺��� ��������

		
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
	//�ӽ� ��ġ ����
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
		ArrayAircrafts[Index]->Destroy();		// ���Ŀ��� ����Ⱑ �ҳ��鼭 �߷� �����ϵ����ϴ� �Լ� ȣ��
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
	// ���߿� ���Ͽ��� ��ġ�� �޾ƿ����� ����
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
