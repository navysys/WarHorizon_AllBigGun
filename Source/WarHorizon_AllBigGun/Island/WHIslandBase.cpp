// Fill out your copyright notice in the Description page of Project Settings.


#include "Island/WHIslandBase.h"

// Sets default values
AWHIslandBase::AWHIslandBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWHIslandBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWHIslandBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

