// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret/WHShell.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controller/WHPlayerController.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"
#include "WaterBodyCustomActor.h"


// Sets default values
AWHShell::AWHShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMeshObject(TEXT("StaticMesh'/Game/Resource/StaticMesh/SM_Shell'"));
	if (DefaultMeshObject.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(DefaultMeshObject.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Shell No Static Mesh"));
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TailObject(TEXT("NiagaraSystem'/Game/Resource/Niagara/NS_ShellTail'"));
	if (TailObject.Succeeded())
	{
		TailEffect = TailObject.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Shell No Effect : Tail"));
	}

	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitObject(TEXT("NiagaraSystem'/Game/Resource/Niagara/NS_HitEffect'"));
	//if (HitObject.Succeeded())
	//{
	//	HitEffect = HitObject.Object;
	//}
	//else
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("Shell No Effect : Hit"));
	//}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> WaterHitObject(TEXT("NiagaraSystem'/Game/Resource/Niagara/NS_WaterHit'"));
	if (WaterHitObject.Succeeded())
	{
		WaterHitEffect = WaterHitObject.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Shell No Effect : WaterHit"));
	}

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleComp->SetRelativeLocation(FVector(-10.0f, 0, 24.0f));
	CapsuleComp->SetRelativeRotation(FRotator(90.0f, 0, 0));
	CapsuleComp->SetCapsuleSize(15.0f, 55.0f);
	CapsuleComp->SetupAttachment(RootComponent);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComp->SetCollisionProfileName(TEXT("TeamAShellPreset"));

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->bRotationFollowsVelocity = true;

}

void AWHShell::PreInitializeComponents()
{
	
	AController* InstigatorController = GetInstigatorController();
	if (InstigatorController != nullptr)
	{
		AWHPlayerController* PlayerController = Cast<AWHPlayerController>(InstigatorController);
		ProjectileMovementComp->InitialSpeed = PlayerController->ShellSpeed;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Shell PreInit Instigator Controller nullptr"));
	}
}

// Called when the game starts or when spawned
void AWHShell::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AWHShell::OnOverlapBegin);

	UNiagaraFunctionLibrary::SpawnSystemAttached(TailEffect, StaticMeshComp, FName(TEXT("Tail")), FVector(0), FRotator(), EAttachLocation::KeepRelativeOffset, true, true);
}

// Called every frame
void AWHShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWHShell::Init(APawn* Player, APawn* PlayerTurret, float Dmg, FVector InitSpeed)
{
	BaseBattleShip = Player;
	BaseTurret = PlayerTurret;
	Damage = Dmg;
	Speed = InitSpeed;
}

void AWHShell::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Shell : BeginOverlap %s"), *OtherComp->GetFName().ToString());

	if (OtherActor->ActorHasTag(TEXT("BattleShip")))
	{
		if (WaterHitEffect) // 현재는 물 이펙트
		{
			// 나중에는 연기 이펙트 현재는 물
			ANiagaraActor* NiagaraActor = GetWorld()->SpawnActor<ANiagaraActor>(ANiagaraActor::StaticClass(), SweepResult.ImpactPoint, FRotator::ZeroRotator);
			if (NiagaraActor)
			{
				NiagaraActor->SetActorLocation(GetActorLocation());
				NiagaraActor->GetNiagaraComponent()->SetAsset(WaterHitEffect);
				NiagaraActor->GetNiagaraComponent()->Activate(true);
			}
		}
	}
	else
	{
		if (WaterHitEffect)
		{
			ANiagaraActor* NiagaraActor = GetWorld()->SpawnActor<ANiagaraActor>(ANiagaraActor::StaticClass(), SweepResult.ImpactPoint, FRotator::ZeroRotator);
			if (NiagaraActor)
			{
				NiagaraActor->SetActorLocation(GetActorLocation());
				NiagaraActor->GetNiagaraComponent()->SetAsset(WaterHitEffect);
				NiagaraActor->GetNiagaraComponent()->Activate(true);
			}
		}
	}

	Destroy();
}

