
#include "Turret/WHShell.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Controller/WHPlayerController.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"
#include "WaterBodyCustomActor.h"
#include "Turret/WHNormalTurretBase.h"


AWHShell::AWHShell()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxComp;
	BoxComp->SetBoxExtent(FVector(550.f, 15.0f, 15.0f));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComp->SetCollisionProfileName(TEXT("TeamAShellPreset"));

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComp->SetupAttachment(RootComponent);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->bRotationFollowsVelocity = true;

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AWHShell::OnOverlapBegin);
}

void AWHShell::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (AWHNormalTurretBase* Turret = Cast<AWHNormalTurretBase>(Owner))
	{
		ProjectileMovementComp->InitialSpeed = Turret->GetShellVelocity();
	}
}

void AWHShell::BeginPlay()
{
	Super::BeginPlay();

	if (TailEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(TailEffect, StaticMeshComp, FName(TEXT("Tail")), FVector(0), FRotator(), EAttachLocation::KeepRelativeOffset, true, true);
	}
}

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
	if (OtherActor->ActorHasTag(TEXT("BattleShip")))
	{
		if (HitEffect)
		{
			ANiagaraActor* NiagaraActor = GetWorld()->SpawnActor<ANiagaraActor>(ANiagaraActor::StaticClass(), SweepResult.ImpactPoint, FRotator::ZeroRotator);
			if (NiagaraActor)
			{
				NiagaraActor->SetActorLocation(GetActorLocation());
				NiagaraActor->GetNiagaraComponent()->SetAsset(HitEffect);
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

