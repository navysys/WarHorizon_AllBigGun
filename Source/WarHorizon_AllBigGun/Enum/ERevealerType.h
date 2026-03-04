#pragma once
#include "CoreMinimal.h"

UENUM()
enum class ERevealerType : uint8
{
	BattleShip = 0 UMETA(DisplayName = "BattleShip"),
	Aircraft UMETA(DisplayName = "Aircraft"),
	Island UMETA(DisplayName = "Island"),
	Invalid
};