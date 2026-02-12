#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ETurretState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Delay UMETA(DisplayName = "Normal Delay"),
	Turn UMETA(DisplayName = "Turn"),
	Ready UMETA(DisplayName = "Ready To Fire"),
	Fire UMETA(DisplayName = "Fire"),
	AfterDelay UMETA(DisplayName = "Fire AfterDelay"),
	Attack UMETA(DisplayName = "Attack Command"),
	WaitingAttack UMETA(DisplayName = "Waiting Attack Command"),
	Stop UMETA(DisplayName = "Cancel Command"),
	AllClear UMETA(DisplayName = "Turret Handler Final Command"),
	Invalid
};