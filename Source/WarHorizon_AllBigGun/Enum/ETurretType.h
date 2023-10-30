
#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ETurretType : uint8
{
	Main = 1 UMETA(DisplayName = "Main Turret"),
	Torpedo UMETA(DisplayName = "Torpedo Launcher"),
	Sub UMETA(DisplayName = "Sub Turret"),
	Air UMETA(DisplayName = "Air Turret"),
	DualPurpose UMETA(DisplayName = "DualPurpose Turret"),
	Invalid
};