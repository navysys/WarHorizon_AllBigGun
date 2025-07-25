#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ETurretState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Turn UMETA(DisplayName = "Turn"),
	Stop UMETA(DisplayName = "Cancel Command"),
	Ready UMETA(DisplayName = "Ready To Fire"),
	Fire UMETA(DisplayName = "Fire and AfterDelay"),
	Invalid
};