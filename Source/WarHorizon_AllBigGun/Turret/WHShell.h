#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WHShell.generated.h"

class UCapsuleComponent;
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
	//class AWHPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> TailNiagaraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraSystem> TailEffect;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraSystem> HitEffect;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraSystem> WaterHitEffect;


	UPROPERTY(VisibleAnywhere)
	APawn* BaseBattleShip;
	UPROPERTY(VisibleAnywhere)
	APawn* BaseTurret;

	UPROPERTY(VisibleAnywhere)
	float Damage;
	UPROPERTY(VisibleAnywhere)
	FVector Speed;

};
