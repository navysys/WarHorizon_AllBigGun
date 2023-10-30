// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleShip/WHBattleShip.h"
#include "Game/WHGameSingleton.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Turret/WHTurret.h"
#include "Engine/StaticMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

// Sets default values
AWHBattleShip::AWHBattleShip()
{
	PrimaryActorTick.bCanEverTick = true;


	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetCollisionProfileName(TEXT("BattleShip"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMeshObject(TEXT("StaticMesh'/Game/Resource/SM_YamatoBase'"));
	if (DefaultMeshObject.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(DefaultMeshObject.Object);
	}

	FloatingPawnMove = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	FloatingPawnMove->TurningBoost = 1;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetRelativeRotation(FRotator(-50.0f, 90.0f, 0.0f));
	SpringArmComp->TargetArmLength = 50000.0f;
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritYaw = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->SetupAttachment(StaticMeshComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void AWHBattleShip::BeginPlay()
{
	Super::BeginPlay();


	LoadDataTableToName((TEXT("Yamato")));		// 테스트용 현재 모델링이 하나 밖에 없음..
	CreateTurretToMeshCompSocket(StaticMeshComp, FName(BattleShipName));

	GetWorld()->GetTimerManager().SetTimer(DetectTimerHandle, this, &AWHBattleShip::DetectBattleShip, 0.2f, true);
}

void AWHBattleShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward();
	ChangeRotation(DeltaTime);

}

//void AWHBattleShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

void AWHBattleShip::UserAttack()
{
}

void AWHBattleShip::UserAttackCancel()
{
}

void AWHBattleShip::UserSkill1()
{
}

void AWHBattleShip::UserSkill2()
{
}

void AWHBattleShip::UserSkill3()
{
}

void AWHBattleShip::UserSkill4()
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
	FVector Point = FVector(HitPoint.X, HitPoint.Y, 0.0f);
	FVector Start = FVector(GetTransform().GetLocation().X, GetTransform().GetLocation().Y, 0.0f);

	FVector Dir = Point - Start;

	float Dot = FVector::DotProduct(GetActorRightVector(), Dir.GetSafeNormal());			// 현재 모델링이 90도 회전한 상태라 RightVector 사용 중
	float AcosAngle = FMath::Acos(Dot);
	float Angle = FMath::RadiansToDegrees(AcosAngle);

	FVector Cross = FVector::CrossProduct(GetActorRightVector(), Dir.GetSafeNormal());	// 현재 모델링이 90도 회전한 상태라 RightVector 사용 중
	if (Cross.Z > 0)
	{
		if (bIsTurnLeft)
		{
			bReverseDirection = true;
		}
		TurnAngle = Angle;
		bIsTurnLeft = false;
	}
	else if (Cross.Z < 0)
	{
		if (!bIsTurnLeft)
		{
			bReverseDirection = true;
		}
		TurnAngle = -Angle;
		bIsTurnLeft = true;
	}
}

void AWHBattleShip::IncreaseMoveSpeed()
{
	MoveSpeed += Acceleration * GetWorld()->DeltaTimeSeconds;
	if (MoveSpeed > InitMaxMoveSpeed)
	{
		MoveSpeed = InitMaxMoveSpeed;
	}
	if (FloatingPawnMove)
	{
		FloatingPawnMove->MaxSpeed = MoveSpeed;
	}
}

void AWHBattleShip::DecreaseMoveSpeed()
{
	MoveSpeed -= Deceleration * GetWorld()->DeltaTimeSeconds;
	if (MoveSpeed < 0)
	{
		MoveSpeed = 0;
	}
	if (FloatingPawnMove)
	{
		FloatingPawnMove->MaxSpeed = MoveSpeed;
	}
}



/*			데이터 관련 함수			*/
void AWHBattleShip::LoadDataTableToName(FName Name)
{
	UDataTable* BattleShipData = UWHGameSingleton::Get().GetBattleShipDataTable();
	FBattleShipDataTable* Table = BattleShipData->FindRow<FBattleShipDataTable>(Name, "");

	if (Table)
	{
		ID = Table->ID;
		BattleShipName = Table->Name;
		BattleShipType = Table->Type;
		BaseMesh = Table->BaseMesh;

		HP = Table->HP;
		MaxHP = Table->MaxHP;
		MP = Table->MP;
		MaxMP = Table->MaxMP;

		InitMaxMoveSpeed = Table->MoveSpeed;
		InitAcceleration = Table->Acceleration;
		InitMaxRotationSpeed = Table->RotationSpeed;
		InitRotationAcceleration = Table->RotationAcceleration;

		InitStat();
	}
}

void AWHBattleShip::InitStat()
{

	MoveSpeed = 0;
	Acceleration = InitAcceleration;
	Deceleration = Acceleration;
	FloatingPawnMove->MaxSpeed = MoveSpeed;
	FloatingPawnMove->Acceleration = InitAcceleration;
	FloatingPawnMove->Deceleration = Deceleration;

	TurnAngle = 0;
	RotationSpeed = 0;
	MaxRotationSpeed = InitMaxRotationSpeed;
	RotationAcceleration = InitRotationAcceleration;

	bCanMove = true;
	bCanRotation = true;
	bReverseDirection = false;
	bIsRotationDeceleration = false;
	bIsTurnLeft = false;

	StaticMeshComp->SetStaticMesh(BaseMesh);

}
// 매시 컴포넌트를 인자로 받아 소켓의 위치 방향을 받아 포탑을 생성
void AWHBattleShip::CreateTurretToMeshCompSocket(UStaticMeshComponent* MeshComp, FName ShipName)
{
	FString BSName = ShipName.ToString();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	TArray<UStaticMeshSocket*> MainTurretSockets = MeshComp->GetStaticMesh()->GetSocketsByTag(TEXT("MainTurret"));
	TArray<UStaticMeshSocket*> SubTurretSockets1 = MeshComp->GetStaticMesh()->GetSocketsByTag(TEXT("SubTurret1"));
	TArray<UStaticMeshSocket*> AirTurretSockets1 = MeshComp->GetStaticMesh()->GetSocketsByTag(TEXT("AirTurret1"));
	TArray<UStaticMeshSocket*> AirTurretSockets2 = MeshComp->GetStaticMesh()->GetSocketsByTag(TEXT("AirTurret2"));
	TArray<UStaticMeshSocket*> AirTurretSockets3 = MeshComp->GetStaticMesh()->GetSocketsByTag(TEXT("AirTurret3"));
	TArray<UStaticMeshSocket*> AirTurretSockets4 = MeshComp->GetStaticMesh()->GetSocketsByTag(TEXT("AirTurret4"));


	if (MainTurretSockets.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_MT");
		for (int i = 0; i < MainTurretSockets.Num(); i++)
		{
			AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(MainTurretSockets[i]->RelativeLocation, FRotator::ZeroRotator, SpawnParams);
			SpawnedTurret->SetupTurret(this, FName(TurretDataName));
			SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *MainTurretSockets[i]->SocketName.ToString());

			AllTurrets.Emplace(SpawnedTurret);
			MainTurrets.Emplace(SpawnedTurret);
		}
	}

	if (SubTurretSockets1.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_ST_1");
		for (int i = 0; i < SubTurretSockets1.Num(); i++)
		{
			AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(SubTurretSockets1[i]->RelativeLocation, FRotator::ZeroRotator, SpawnParams);
			SpawnedTurret->SetupTurret(this, FName(TurretDataName));
			SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *SubTurretSockets1[i]->SocketName.ToString());

			AllTurrets.Emplace(SpawnedTurret);
			SubTurrets1.Emplace(SpawnedTurret);
		}
	}

	if (AirTurretSockets1.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_AT_1");
		for (int i = 0; i < AirTurretSockets1.Num(); i++)
		{
			AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(AirTurretSockets1[i]->RelativeLocation, FRotator::ZeroRotator, SpawnParams);
			SpawnedTurret->SetupTurret(Cast<APawn>(this), FName(TurretDataName));
			SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *AirTurretSockets1[i]->SocketName.ToString());

			AllTurrets.Emplace(SpawnedTurret);
			AirTurrets1.Emplace(SpawnedTurret);
		}
	}

	if (AirTurretSockets2.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_AT_2");
		for (int i = 0; i < AirTurretSockets2.Num(); i++)
		{
			AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(AirTurretSockets2[i]->RelativeLocation, FRotator::ZeroRotator, SpawnParams);
			SpawnedTurret->SetupTurret(Cast<APawn>(this), FName(TurretDataName));
			SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *AirTurretSockets2[i]->SocketName.ToString());

			AllTurrets.Emplace(SpawnedTurret);
			AirTurrets2.Emplace(SpawnedTurret);
		}
	}

	if (AirTurretSockets3.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_AT_3");
		for (int i = 0; i < AirTurretSockets3.Num(); i++)
		{
			AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(AirTurretSockets3[i]->RelativeLocation, FRotator::ZeroRotator, SpawnParams);
			SpawnedTurret->SetupTurret(Cast<APawn>(this), FName(TurretDataName));
			SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *AirTurretSockets3[i]->SocketName.ToString());

			AllTurrets.Emplace(SpawnedTurret);
			AirTurrets3.Emplace(SpawnedTurret);
		}
	}

	if (AirTurretSockets4.Num() > 0)
	{
		FString TurretDataName = BSName + TEXT("_AT_4");
		for (int i = 0; i < AirTurretSockets4.Num(); i++)
		{
			AWHTurret* SpawnedTurret = GetWorld()->SpawnActor<AWHTurret>(AirTurretSockets4[i]->RelativeLocation, FRotator::ZeroRotator, SpawnParams);
			SpawnedTurret->SetupTurret(Cast<APawn>(this), FName(TurretDataName));
			SpawnedTurret->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *AirTurretSockets4[i]->SocketName.ToString());

			AllTurrets.Emplace(SpawnedTurret);
			AirTurrets4.Emplace(SpawnedTurret);
		}
	}

}




/*			움직임 회전 함수			*/
void AWHBattleShip::MoveForward()
{
	if (bCanMove)
	{
		AddMovementInput(GetActorRightVector());		// 현재 모델링이 90도 회전한 상태라 RightVector 사용 중
	}
}

void AWHBattleShip::ChangeRotation(float DeltaTime)
{

	if (bCanRotation)	// 회전 운동, 계산을 아예 하지 않으려면 false
	{
		float AbsTurnAngle = FMath::Abs(TurnAngle);

		// 반대 방향으로 회전할 때 천천히 회전하기 위한 bReverseDirection 확인
		if (bReverseDirection)
		{
			if (RotationSpeed > 0)
			{
				RotationSpeed -= RotationAcceleration * 2 * DeltaTime;

				// 역방향으로 방향을 전환할 경우 RotationSpeed가 0으로 감소된 이후 bReverseDirection 변경
				if (RotationSpeed <= 0)
				{
					RotationSpeed = 0;
					bReverseDirection = false;
				}
			}
			else
			{
				// 처음 방향전환을 실행할 경우 실행
				bReverseDirection = false;
			}
		}
		else if (bIsRotationDeceleration)
		{
			if (AbsTurnAngle > RotationSpeed)
			{
				bIsRotationDeceleration = false;
			}
			else
			{
				RotationSpeed = AbsTurnAngle;
			}

			if (RotationSpeed <= 0)
			{
				RotationSpeed = 0;
				bIsRotationDeceleration = false;
			}
		}
		// 역방향 이동이 아니고 감속하지 않아야 할 경우 정상적인 회전속도 증가
		else if (TurnAngle != 0)
		{
			if (MaxRotationSpeed > RotationSpeed)
			{
				RotationSpeed += RotationAcceleration * DeltaTime;

				if (MaxRotationSpeed < RotationSpeed)
				{
					RotationSpeed = MaxRotationSpeed;
				}
			}
			else if (MaxRotationSpeed < RotationSpeed)
			{
				RotationSpeed = MaxRotationSpeed;
			}
		}

		// TurnAngle을 기준으로 얼마나 회전해야하는지 계산
		if (!bReverseDirection)
		{
			if (TurnAngle > 0)
			{
				TurnAngle -= RotationSpeed * DeltaTime;
				if (TurnAngle <= 0)
				{
					TurnAngle = 0;
				}
			}
			else if (TurnAngle < 0)
			{
				TurnAngle += RotationSpeed * DeltaTime;
				if (TurnAngle >= 0)
				{
					TurnAngle = 0;
				}
			}
		}

		if (AbsTurnAngle < RotationSpeed)
		{
			bIsRotationDeceleration = true;
		}
		if (bIsRotationDeceleration)
		{
			if (AbsTurnAngle > RotationSpeed)
			{
				bIsRotationDeceleration = false;
			}
		}

		// TurnAngle을 토대로 Actor의 방향을 직접 회전
		if (TurnAngle != 0)
		{
			// 이전 프레임과 같은 정방향 회전하는 경우 실행
			if (!bReverseDirection)
			{
				if (TurnAngle > 0)
				{
					AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
				}
				else
				{
					AddActorLocalRotation(FRotator(0.0f, RotationSpeed * -1.0f * DeltaTime, 0.0f));
				}
			}
			// 이전 프레임과 반대 방향으로 진행해야하는 경우 실행
			else
			{
				if (TurnAngle < 0)
				{
					AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
				}
				else
				{
					AddActorLocalRotation(FRotator(0.0f, RotationSpeed * -1.0f * DeltaTime, 0.0f));
				}
			}

		}
		else
		{
			RotationSpeed = 0;
		}
	}
}





/*			적 탐지 할당 함수			*/
void AWHBattleShip::DetectBattleShip()
{
	float Radius = 40000.0f;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(FName("BattleShip"), false, this);

	bool bResult = GetWorld()->OverlapMultiByProfile(OverlapResults, GetActorLocation(), FQuat::Identity, FName("DetectBattleShip"), FCollisionShape::MakeSphere(Radius), CollisionQueryParam);

	if (bResult)
	{
		TArray<APawn*> ArrayPawn;
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			ArrayPawn.Emplace(Pawn);

			DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 20, FColor::Green, false, 0.2f);
		}
		InRangeShips = ArrayPawn;
		SortingPawnArrayToDistance(InRangeShips);
		AttackTargetShip = InRangeShips[0];
		//SetSubTurretTarget(AttackTargetShip);
	}
	else
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 20, FColor::Red, false, 0.2f);
	}
}

void AWHBattleShip::DetectAircraft()
{

}

void AWHBattleShip::SortingPawnArrayToDistance(TArray<APawn*> ArrayPawn)
{
	ArrayPawn.Sort([this](const APawn& A, const APawn& B) { return A.GetDistanceTo(this) < B.GetDistanceTo(this); });
}
