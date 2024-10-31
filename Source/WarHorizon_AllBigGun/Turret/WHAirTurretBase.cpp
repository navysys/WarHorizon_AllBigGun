
#include "Turret/WHAirTurretBase.h"
#include "Turret/WHBullet.h"

AWHAirTurretBase::AWHAirTurretBase()
{
	TurretType = ETurretType::Air;
}

void AWHAirTurretBase::Fire()
{


	for (int i = 0; i < MuzzleComps.Num(); i++)
	{
		FTransform MuzzleTransform = MuzzleComps[i]->GetComponentTransform();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *MuzzleTransform.GetRotation().ToString());
		GetWorld()->SpawnActor<AWHBullet>(Bullet, MuzzleTransform.GetLocation(), FRotator(HorizontalAngle, StaticMeshComp->GetComponentRotation().Yaw, 0.0f));
	}
}
