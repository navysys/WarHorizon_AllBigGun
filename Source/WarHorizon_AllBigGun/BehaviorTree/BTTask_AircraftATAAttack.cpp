// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTTask_AircraftATAAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/AircraftsInterface.h"
#include "BehaviorTree/AircraftsAI.h"
#include "Enum/EAircraftsState.h"

UBTTask_AircraftATAAttack::UBTTask_AircraftATAAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_AircraftATAAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//타겟 Aircrafts 클래스의 비행기 위치 가져와서 가장 가까운 적 Aircraft 클래스 바라보도록 아군 Aircraft 클래스 상태 변경
	//Aircraft 클래스와 일직선 상에 있다면 공격(이펙트만 나오고 공격과 피격 동시)
	//평상시에도 일직선 상에 있으면 공격

	return EBTNodeResult::Type();
}

void UBTTask_AircraftATAAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//TargetAircrafts 사라진다면 상태 변경

	//AircraftsState 가 달라진다면
	//아군 Aircraft 클래스 상태전환
}

