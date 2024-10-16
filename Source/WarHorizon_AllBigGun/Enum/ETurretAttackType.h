
#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ETurretAttackType : uint8
{
	Wait = 0 UMETA(DisplayName = "Wait for Change Type"),
	PointAttack = 1 UMETA(DisplayName = "Attack to Point"),
	TargetAttack = 2 UMETA(DisplayName = "Attack to Target"),
	Invalid
};