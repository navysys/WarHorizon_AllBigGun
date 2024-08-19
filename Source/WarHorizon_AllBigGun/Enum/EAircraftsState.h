#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EAircraftsState : uint8
{
	MoveStraight = 1 UMETA(DisplayName = "Move Straight State"),
	MoveNormal UMETA(DisplayName = "Move Normal State"),
	Wait UMETA(DisplayName = "Wait State"),
	AtA UMETA(DisplayName = "Air to Air Attack State"),
	AtB UMETA(DisplayName = "Air to BattleShip Attack State"),
	Invalid
};