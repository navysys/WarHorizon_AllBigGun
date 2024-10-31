// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WHBullet.generated.h"


class UNiagaraComponent;


UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWHBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComp;
};
