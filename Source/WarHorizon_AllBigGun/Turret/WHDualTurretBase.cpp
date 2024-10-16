// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret/WHDualTurretBase.h"

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
