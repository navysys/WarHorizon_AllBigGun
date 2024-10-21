#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Enum/EBattleShipType.h"
#include "WHPlayerState.generated.h"


UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	// Stat
protected:
	// UI 변경은 state 변수 기준 바인드
	UPROPERTY(VisibleAnywhere, Category = "Player")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	FString BattleShipName;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	EBattleShipType BattleShipType = EBattleShipType::Invalid;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	uint8 TeamInt = 1;


	UPROPERTY(EditAnywhere, Category = "Stat")
	uint32 MaxHP = 100;

	UPROPERTY(EditAnywhere, Category = "Stat")
	uint32 HP = 100;

	UPROPERTY(EditAnywhere, Category = "Stat")
	uint32 MaxMP = 100;

	UPROPERTY(EditAnywhere, Category = "Stat")
	uint32 MP = 0;

	UPROPERTY(EditAnywhere, Category = "Stat")
	uint16 Defense = 0;

	UPROPERTY(EditAnywhere, Category = "Stat | Attack")
	uint16 AttackPower = 100;

	UPROPERTY(EditAnywhere, Category = "Stat | Attack")
	uint8 AttackRange = 100;

	UPROPERTY(EditAnywhere, Category = "Stat | Attack")
	uint8 AttakReload = 100;

	UPROPERTY(EditAnywhere, Category = "Stat | Attack")
	uint8 DefenseIgnore = 0;

	UPROPERTY(EditAnywhere, Category = "Stat | Attack")
	uint8 CriticalValue = 0;

	UPROPERTY(EditAnywhere, Category = "Stat | Attack")
	uint16 CriticalCoefficient = 100;

	UPROPERTY(EditAnywhere, Category = "Stat | Attack")
	uint8 SkillCooldown = 100;

	UPROPERTY(EditAnywhere, Category = "Stat | Movement")
	float MoveSpeed = 1;

	UPROPERTY(EditAnywhere, Category = "Stat | Movement")
	float MoveAcceleration = 1;

	UPROPERTY(EditAnywhere, Category = "Stat | Movement")
	float RotationSpeed = 1;

	UPROPERTY(EditAnywhere, Category = "Stat | Movement")
	float RotationAcceleration = 1;


};
