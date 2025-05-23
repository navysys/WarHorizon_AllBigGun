#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enum/EBattleShipType.h"
#include "BattleShipDataTable.generated.h"


USTRUCT(BlueprintType)
struct WARHORIZON_ALLBIGGUN_API FBattleShipDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	// 함선 식별 번호
	UPROPERTY(EditAnywhere)
	uint16 ID = 0;
	// 함선 이름
	UPROPERTY(EditAnywhere)
	FString Name = TEXT("None");
	// 함종 구분
	UPROPERTY(EditAnywhere)
	EBattleShipType Type = EBattleShipType::Invalid;

	// 소켓을 포함하는 함체 매시
	UPROPERTY(EditAnywhere)
	USkeletalMesh* BaseMesh = nullptr;

	UPROPERTY(EditAnywhere)
	float HP = 9999.9999f;
	UPROPERTY(EditAnywhere)
	float MaxHP = 9999.9999f;
	UPROPERTY(EditAnywhere)
	float MP = 0.0f;
	UPROPERTY(EditAnywhere)
	float MaxMP = 0.0f;

	// 최대 이동속도
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 0.0f;
	// 가속도
	UPROPERTY(EditAnywhere)
	float Acceleration = 0.0f;

	// 최대 회전속도
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 0.0f;
	// 회전 가속도
	UPROPERTY(EditAnywhere)
	float RotationAcceleration = 0.0f;
	// 회전가속 증가량
	UPROPERTY(EditAnywhere)
	float RotationAccelerationIncrease = 0.0f;

	UPROPERTY(EditAnywhere)
	int AircraftsID = 0;

	UPROPERTY(EditAnywhere)
	int AircraftsID2 = 0;

	UPROPERTY(EditAnywhere)
	int AircraftsID3 = 0;

	UPROPERTY(EditAnywhere)
	int AircraftsID4 = 0;
};
