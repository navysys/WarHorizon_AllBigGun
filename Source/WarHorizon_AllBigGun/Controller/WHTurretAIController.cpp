// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WHTurretAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/TurretAI.h"

AWHTurretAIController::AWHTurretAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_Turret'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_Turret'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AWHTurretAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsBool(BBKEY_ISRELOAD, true);
		Blackboard->SetValueAsBool(BBKEY_ISLOOKAT, false);
		Blackboard->SetValueAsInt(BBKEY_ATTACKTYPE, 0);
		Blackboard->SetValueAsFloat(BBKEY_RELOADTIME, 5.0f);
		Blackboard->SetValueAsFloat(BBKEY_MAXANGLE, 0.0f);

		Blackboard->SetValueAsFloat(BBKEY_INITROTATION, 9999.9f);


		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AWHTurretAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AWHTurretAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// 여기서 폰을 인터페이스로 변환하고 데이터 받아오기

	RunAI();
}
