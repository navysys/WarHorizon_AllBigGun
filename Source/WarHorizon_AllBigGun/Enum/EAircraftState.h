#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EAircraftState : uint8
{
	Landing = 1 UMETA(DisplayName = "Landing : Spawn and Move Stright"),
	FollowGroup UMETA(DisplayName = "Follow : Follow Group high speed"),
	Normal UMETA(DisplayName = "Normal : Just Move Front and Turn"),
	AtA UMETA(DisplayName = "AtA : Chase Enemy Aircraft"),
	AtB UMETA(DisplayName = "AtB : Attack BattleShip"),
	Invalid
};