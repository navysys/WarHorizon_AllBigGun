// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret/WHBullet.h"
#include "NiagaraComponent.h"

// Sets default values
AWHBullet::AWHBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMeshComp;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	NiagaraComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWHBullet::BeginPlay()
{
	Super::BeginPlay();
	
	//NiagaraComp->active
}

// Called every frame
void AWHBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

