
#include "Game/WHGameInstance.h"

UWHGameInstance::UWHGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> BattleShipDataRef(TEXT("/Game/DataTable/DT_BattleShipData"));
	if (nullptr != BattleShipDataRef.Object)
	{
		BattleShipDataTable = BattleShipDataRef.Object;
		check(BattleShipDataTable->GetRowMap().Num() > 0);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> TurretDataRef(TEXT("/Game/DataTable/DT_TurretData"));
	if (nullptr != TurretDataRef.Object)
	{
		TurretDataTable = TurretDataRef.Object;
		check(TurretDataTable->GetRowMap().Num() > 0);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> AircraftDataRef(TEXT("/Game/DataTable/DT_AircraftData"));
	if (nullptr != AircraftDataRef.Object)
	{
		AircraftDataTable = AircraftDataRef.Object;
		check(AircraftDataTable->GetRowMap().Num() > 0);
	}


}
