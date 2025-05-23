
#include "Component/WHCSkillHandler.h"
#include "Controller/WHPlayerController.h"
#include "Skill/WHSkillBase.h"


UWHCSkillHandler::UWHCSkillHandler()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWHCSkillHandler::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UWHCSkillHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWHCSkillHandler::UseSkill(char Key)
{
	AWHPlayerController* PC = Cast<AWHPlayerController>(GetOwner());
	if (IsValid(PC))
	{
		IBattleShipInterface* IBS = PC->BattleShipPawn;
		if (IBS != nullptr)
		{
			UWHSkillBase* Skill = Cast<UWHSkillBase>(IBS->GetSkill(Key));
			if (IsValid(Skill))
			{
				// 마나, 쿨타임, 스킬 사용 가능한 상태인지 다 체크

				// 마나, 쿨타임 적용

				Skill->Effect();
			}
		}
	}

	//if (Key == 'Q')
	//{
	//	SkillQ->Effect();
	//}
	//else if (Key == 'W')
	//{
	//	SkillW->Effect();
	//}
	//else if (Key == 'E')
	//{
	//	SkillE->Effect();
	//}
	//else if (Key == 'R')
	//{
	//	SkillR->Effect();
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Skill Handler -> UseSkill : 알 수 없는 키값"));
	//}
}

