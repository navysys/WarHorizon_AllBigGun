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

	//Ÿ�� Aircrafts Ŭ������ ����� ��ġ �����ͼ� ���� ����� �� Aircraft Ŭ���� �ٶ󺸵��� �Ʊ� Aircraft Ŭ���� ���� ����
	//Aircraft Ŭ������ ������ �� �ִٸ� ����(����Ʈ�� ������ ���ݰ� �ǰ� ����)
	//���ÿ��� ������ �� ������ ����

	return EBTNodeResult::Type();
}

void UBTTask_AircraftATAAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//TargetAircrafts ������ٸ� ���� ����

	//AircraftsState �� �޶����ٸ�
	//�Ʊ� Aircraft Ŭ���� ������ȯ
}

