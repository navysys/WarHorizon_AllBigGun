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
	void Turn(float DeltaTime);
	void Stop(float DeltaTime);
	void Ready(float DeltaTime);
	void Fire(float DeltaTime);

private:
	ETurretState CurrentState;
	ETurretState CommandState;
};
