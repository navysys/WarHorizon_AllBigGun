// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/ETurretState.h"
#include "WHCTurretFSM.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARHORIZON_ALLBIGGUN_API UWHCTurretFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWHCTurretFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCommandState(ETurretState State);
	ETurretState GetCurrentState();

protected:
	void Idle(float DeltaTime);
	void Delay(float DeltaTime);
	void Turn(float DeltaTime);
	void Ready(float DeltaTime);
	void Fire(float DeltaTime);
	void AfterDelay(float DeltaTime);
	void Stop(float DeltaTime);
	

private:
	class AWHTurretBase* Turret;

	ETurretState CurrentState;
	ETurretState CommandState;

	float BeforeActionTime = 0.0f;
	float TurnDelay = 0.02f;
	float FireDelay = 0.5f;
};
