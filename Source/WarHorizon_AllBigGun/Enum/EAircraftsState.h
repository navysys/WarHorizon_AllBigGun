#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EAircraftsState : uint8
{
	Turn = 1 UMETA(DisplayName = "Turn to Target State"),
	Move UMETA(DisplayName = "Move Forward State"),
	Wait UMETA(DisplayName = "Wait State"),
	AtA UMETA(DisplayName = "Air to Air Attack State"),
	AtB UMETA(DisplayName = "Air to BattleShip Attack State"),
	Invalid
};