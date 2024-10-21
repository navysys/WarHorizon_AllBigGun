
#include "Turret/WHNormalTurretBase.h"
#include "Kismet/GameplayStatics.h"
#include "Turret/WHShell.h"

AWHNormalTurretBase::AWHNormalTurretBase()
{
	
}

void AWHNormalTurretBase::Fire()
{
	Super::Fire();

	FVector MuzzleLocation = MuzzleComps[0]->GetComponentLocation();
	FVector TargetLoc = MuzzleLocation + StaticMeshComp->GetForwardVector() * TargetData.Distance;  // Ÿ�� ����.
	float ArcValue = 0.65f;                       // ArcParam (0.0-1.0) ��絵 ���߿� Ÿ�� �����Ϳ��� �����ϵ��� ����
	float GravityZ = GetWorld()->GetGravityZ();
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, ShellVelocityVector, MuzzleLocation, TargetLoc, GravityZ, ArcValue))
	{
		FRotator LaunchAngle = ShellVelocityVector.Rotation();
		ShellLaunchAngle = LaunchAngle.Pitch;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ShellVelocity = ShellVelocityVector.Size();

	// ��ź �߻� ����
	FRotator Rot = StaticMeshComp->GetComponentRotation() + FRotator(ShellLaunchAngle, 0.0f, 0.0f);
	for (int i = 0; i < MuzzleComps.Num(); i++)
	{
		AWHShell* SpawnShell = GetWorld()->SpawnActor<AWHShell>(Shell, MuzzleComps[i]->GetComponentLocation(), Rot + FRotator(0.0f, Dispersion[i], 0.0f), SpawnParams);
	}
}
