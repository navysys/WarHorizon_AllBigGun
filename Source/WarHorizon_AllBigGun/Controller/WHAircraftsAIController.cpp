// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WHAircraftsAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/AircraftsAI.h"

AWHAircraftsAIController::AWHAircraftsAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_Aircrafts'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_Aircrafts'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AWHAircraftsAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsInt(BBKEY_MAXSPAWNCOUNT, 4);
		Blackboard->SetValueAsInt(BBKEY_CURRENTSPAWNCOUNT, 0);
		Blackboard->SetValueAsBool(BBKEY_ISSPAWNEND, false);
		Blackboard->SetValueAsBool(BBKEY_ISREACHTAGETPOSITION, false);
		Blackboard->SetValueAsBool(BBKEY_ISMOVESTRAIGHT, true);
		Blackboard->SetValueAsObject(BBKEY_MOTHERSHIPACTOR, MotherShipPawn);
		Blackboard->SetValueAsVector(BBKEY_TARGETPOSITION, FVector(10000, 10000, 12000));
		Blackboard->SetValueAsVector(BBKEY_WAITINGPOSITION, FVector(0, 0, 0));

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AWHAircraftsAIController::StopAI()
{
}

void AWHAircraftsAIController::BeginPlay()
{
	Super::BeginPlay();


}

void AWHAircraftsAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	MotherShipPawn = InPawn->GetInstigator();

	RunAI();
}