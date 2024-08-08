// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WHShell.generated.h"

UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHShell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWHShell();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PreInitializeComponents() override;

	void Init(APawn* Player, APawn* PlayerTurret, float Dmg, FVector InitSpeed);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	//class AWHPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> CapsuleComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> TailNiagaraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraSystem> TailEffect;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraSystem> HitEffect;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraSystem> WaterHitEffect;


	UPROPERTY(VisibleAnywhere)
	APawn* BaseBattleShip;
	UPROPERTY(VisibleAnywhere)
	APawn* BaseTurret;

	UPROPERTY(VisibleAnywhere)
	float Damage;
	UPROPERTY(VisibleAnywhere)
	FVector Speed;

};
