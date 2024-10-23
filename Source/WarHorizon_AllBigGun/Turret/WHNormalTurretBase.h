#pragma once

#include "CoreMinimal.h"
#include "Turret/WHTurretBase.h"
#include "WHNormalTurretBase.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHNormalTurretBase : public AWHTurretBase
{
	GENERATED_BODY()
	
public:
	AWHNormalTurretBase();
protected:
	virtual void BeginPlay() override;
	virtual void Fire() override;

public:
	float GetShellVelocity() { return ShellVelocity; }
	// 포탄 발사를 위한 변수
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWHShell> Shell;

	UPROPERTY(EditAnywhere)
	TArray<float> Dispersion;

	UPROPERTY(VisibleAnywhere)
	FVector ShellVelocityVector = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere)
	float ShellVelocity = 0.0f;

	UPROPERTY(EditAnywhere)
	float ShellLaunchAngle;
};
