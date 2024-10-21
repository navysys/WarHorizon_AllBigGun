
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
	FVector TargetLoc = MuzzleLocation + StaticMeshComp->GetForwardVector() * TargetData.Distance;  // 타겟 지점.
	float ArcValue = 0.65f;                       // ArcParam (0.0-1.0) 경사도 나중에 타겟 셀렉터에서 설정하도록 수정
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

	// 포탄 발사 관련
	FRotator Rot = StaticMeshComp->GetComponentRotation() + FRotator(ShellLaunchAngle, 0.0f, 0.0f);
	for (int i = 0; i < MuzzleComps.Num(); i++)
	{
		AWHShell* SpawnShell = GetWorld()->SpawnActor<AWHShell>(Shell, MuzzleComps[i]->GetComponentLocation(), Rot + FRotator(0.0f, Dispersion[i], 0.0f), SpawnParams);
	}
}
