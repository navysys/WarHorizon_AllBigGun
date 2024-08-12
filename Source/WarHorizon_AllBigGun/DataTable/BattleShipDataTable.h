// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enum/EBattleShipType.h"
#include "Skill/WHSkillBase.h"
#include "BattleShipDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct WARHORIZON_ALLBIGGUN_API FBattleShipDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	// 함선 식별 번호
	UPROPERTY(EditAnywhere)
	uint16 ID;
	// 함선 이름
	UPROPERTY(EditAnywhere)
	FString Name;
	// 함종 구분
	UPROPERTY(EditAnywhere)
	EBattleShipType Type;

	// 소켓을 포함하는 함체 매시
	UPROPERTY(EditAnywhere)
	USkeletalMesh* BaseMesh;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UObject> SkillQ;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UObject> SkillW;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UObject> SkillE;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UObject> SkillR;

	UPROPERTY(EditAnywhere)
	float HP;
	UPROPERTY(EditAnywhere)
	float MaxHP;
	UPROPERTY(EditAnywhere)
	float MP;
	UPROPERTY(EditAnywhere)
	float MaxMP;

	// 최대 이동속도
	UPROPERTY(EditAnywhere)
	float MoveSpeed;
	// 가속도
	UPROPERTY(EditAnywhere)
	float Acceleration;

	// 최대 회전속도
	UPROPERTY(EditAnywhere)
	float RotationSpeed;
	// 회전 가속도
	UPROPERTY(EditAnywhere)
	float RotationAcceleration;
	// 회전가속 증가량
	UPROPERTY(EditAnywhere)
	float RotationAccelerationIncrease;

	UPROPERTY(EditAnywhere)
	int AircraftsID = 0;

	UPROPERTY(EditAnywhere)
	int AircraftsID2 = 0;

	UPROPERTY(EditAnywhere)
	int AircraftsID3 = 0;

	UPROPERTY(EditAnywhere)
	int AircraftsID4 = 0;
};
