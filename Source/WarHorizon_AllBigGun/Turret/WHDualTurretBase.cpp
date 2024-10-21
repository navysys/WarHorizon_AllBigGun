
#include "Turret/WHDualTurretBase.h"

AWHDualTurretBase::AWHDualTurretBase()
{
	TurretType = ETurretType::DualPurpose;
}

void AWHDualTurretBase::Fire()
{
	Super::Fire();
	switch (DualState)
	{
	case DualTurretState::Normal:
		break;
	case DualTurretState::Air:
		break;
	case DualTurretState::Invalid:
		break;
	default:
		break;
	}
}
