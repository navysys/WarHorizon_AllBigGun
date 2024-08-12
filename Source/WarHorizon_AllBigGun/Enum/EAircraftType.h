
#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EAircraftType : uint8
{
	Fighter = 1 UMETA(DisplayName = "Air to Air"),
	DiveBomber UMETA(DisplayName = "Dive Bomber"),
	HorizontalBomber UMETA(DisplayName = "Horizontal Bomber"),
	TorpedoBomber UMETA(DisplayName = "Torpedo Bomber"),
	PatrolAircraft UMETA(DisplayName = "Patrol Aircraft"),
	Invalid
};