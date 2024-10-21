#pragma once

#include "CoreMinimal.h"


UENUM()
enum class EBattleShipType : uint8
{
	DD = 1 UMETA(DisplayName = "Destroyers"),
	CL UMETA(DisplayName = "Light Cruiser"),
	CA UMETA(DisplayName = "Heavy Cruiser"),
	BB UMETA(DisplayName = "Battleship"),
	CV UMETA(DisplayName = "Aircraft Carrier"),
	Invalid
};