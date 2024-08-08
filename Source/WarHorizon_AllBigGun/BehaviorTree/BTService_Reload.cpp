// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BTService_Reload.h"
#include "TurretAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Reload::UBTService_Reload()
{
}

void UBTService_Reload::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool bIsReload = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISRELOAD);
	float ReloadTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_RELOADTIME);

	if (!bIsReload)
	{
		CurrentTime += DeltaSeconds;

		if (ReloadTime <= CurrentTime)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISRELOAD, true);
			CurrentTime = 0.0f;
		}
	}
	else
	{
		if (CurrentTime != 0.0f)
		{
			CurrentTime = 0.0f;
		}
	}
}
