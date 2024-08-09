// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret/WHTurret.h"
#include "DataTable/TurretDataTable.h"
#include "Game/WHGameSingleton.h"
#include "Controller/WHTurretAIController.h"
#include "Kismet/GameplayStatics.h"
#include "WHShell.h"
#include "Controller/WHPlayerController.h"
#include "NiagaraSystem.h"

// Sets default values
AWHTurret::AWHTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AWHTurretAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> GunFireEffectRef(TEXT("NiagaraSystem'/Game/Resource/Niagara/NS_GunFire'"));
	if (GunFireEffectRef.Succeeded())
	{
		GunFireEffect = GunFireEffectRef.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Shell No Effect : GunFire"));
	}
}

// Called when the game starts or when spawned
void AWHTurret::BeginPlay()
{
	Super::BeginPlay();

}

void AWHTurret::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AAIController* AIController = Cast<AWHTurretAIController>(GetController());
	if (AIController)
	{
		AIController->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}

	AController* InstigatorController = GetInstigatorController();
	if (InstigatorController != nullptr)
	{
		PlayerController = Cast<AWHPlayerController>(InstigatorController);
	}

	
}

// Called every frame
void AWHTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AWHTurret::Fire()
{
	Super::Fire();

	if (PlayerController == nullptr)
	{
		PlayerController = Cast<AWHPlayerController>(GetInstigatorController());
	}

	FVector MuzzleLocation = GetActorRotation().RotateVector(Muzzles[0]->RelativeLocation);
	FVector TurretLocation = GetActorLocation();
	FVector StartLocation = TurretLocation + MuzzleLocation;

	FVector TargetLoc = FVector(TurretLocation.X, TurretLocation.Y, 0.0f) + GetActorForwardVector() * TargetDistance;  // 타겟 지점.
	float ArcValue = 0.65f;                       // ArcParam (0.0-1.0) 경사도
	float GravityZ = GetWorld()->GetGravityZ();
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, ShellVelocityVector, StartLocation, TargetLoc, GravityZ, ArcValue))
	{
		FRotator LaunchAngle = ShellVelocityVector.Rotation();
		ShellLaunchAngle = LaunchAngle.Pitch;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ShellVelocity = ShellVelocityVector.Size();
	if (PlayerController != nullptr)
	{
		PlayerController->ShellSpeed = ShellVelocity;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController Nullptr"));
	}

	// 분산도 관련 코드
	int MuzzleInt = Muzzles.Num();
	float DispersionAngle = 0.0f;
	if (MuzzleInt == 4)
	{
		DispersionAngle = 7.5f;
	}
	else if (MuzzleInt == 3)
	{
		DispersionAngle = 5.0f;
	}
	else if (MuzzleInt == 2)
	{
		DispersionAngle = 2.5f;
	}

	for (int i = 1; i <= MuzzleInt; i++)
	{
		Dispersion.Emplace((DispersionAngle / (MuzzleInt - 1)) * (i - ((MuzzleInt + 1) / 2)));
	}

	// 포탄 발사 관련
	FRotator Rot = GetActorRotation() + FRotator(ShellLaunchAngle, 0.0f, 0.0f);
	for (int i = 0; i < MuzzleInt; i++)
	{
		AWHShell* Shell = GetWorld()->SpawnActor<AWHShell>(GetActorLocation() + GetActorRotation().RotateVector(Muzzles[i]->RelativeLocation), Rot + FRotator(0.0f, Dispersion[i], 0.0f), SpawnParams);
		
		// Shell->Init(BaseBattleShip, this, 0, ShellVelocity);
	}

	// 발사 이펙트 관련

}

float AWHTurret::GetMaxHorizontalAngle()
{
	return MaxHorizontalAngle;
}

APawn* AWHTurret::GetBaseBattleShip()
{
	return BaseBattleShip;
}

uint8 AWHTurret::GetAttackType()
{
	return static_cast<uint8>(AttackType);
}

float AWHTurret::GetTurretTargetAngle()
{
	return TargetAngle;
}

APawn* AWHTurret::GetTurretTargetPawn()
{
	return TargetPawn;
}

FVector AWHTurret::GetTurretTargetPoint()
{
	return TargetPoint;
}

float AWHTurret::GetRotationSpeed()
{
	return RotationSpeed;
}

float AWHTurret::GetReloadTime()
{
	return ReloadTime;
}
