#pragma once

#include "CoreMinimal.h"


UENUM()
enum class ETeamType : uint8
{
	Red = 1 UMETA(DisplayName = "Team1Red"),
	Blue UMETA(DisplayName = "Team2Blue"),
	Neutral UMETA(DisplayName = "Team3Neutral"),
	Invalid
};