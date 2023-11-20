
#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ETurretAttackType : uint8
{
	Wait = 0 UMETA(DisplayName = "Wait for Change Type"),
	AngleAttack = 1 UMETA(DisplayName = "Attack to Angle"),
	PointAttack = 2 UMETA(DisplayName = "Attack to Point"),
	TargetAttack = 3 UMETA(DisplayName = "Attack to Target"),
	Invalid
};