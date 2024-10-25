#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WHShell.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;


UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHShell : public AActor
{
	GENERATED_BODY()
	
public:	
	AWHShell();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void PreInitializeComponents() override;

	void Init(APawn* Player, APawn* PlayerTurret, float Dmg, FVector InitSpeed);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> TailNiagaraComp; 

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> TailEffect;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> HitEffect;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> WaterHitEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> HitEffectActor;


	UPROPERTY(VisibleAnywhere)
	APawn* BaseBattleShip;
	UPROPERTY(VisibleAnywhere)
	APawn* BaseTurret;

	UPROPERTY(VisibleAnywhere)
	float Damage;
	UPROPERTY(VisibleAnywhere)
	FVector Speed;

};
