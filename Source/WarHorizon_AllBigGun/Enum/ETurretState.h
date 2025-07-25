#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ETurretState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Turn UMETA(DisplayName = "Turn"),
	Ready UMETA(DisplayName = "Ready To Fire"),
	Fire UMETA(DisplayName = "Fire and AfterDelay"),
	Stop UMETA(DisplayName = "Cancel Command"),
	AllClear UMETA(DisplayName = "Turret Handler Final Command"),
	Invalid
};