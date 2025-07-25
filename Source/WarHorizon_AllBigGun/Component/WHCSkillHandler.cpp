
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
	InitSkillMap();
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
		ISkillUsableInterface* SUI = Cast<ISkillUsableInterface>(PC->GetPawn());
		if (SUI != nullptr)
		{
			UWHSkillBase* Skill = Cast<UWHSkillBase>(SUI->GetSkill(Key));
			if (IsValid(Skill))
			{
				// 함선 상태 체크 (상태이상, 마나)
				
				
				// 쿨타임 체크
				if (!CheckSkillCooldown(Key))
				{
					UE_LOG(LogTemp, Warning, TEXT(" Skill Key %c is on cooldown"), Key);
					return;
				}


				// 마나, 쿨타임 적용
				ApplyCooldown(Key);
				Skill->Effect();
			}
		}
	}
}

void UWHCSkillHandler::InitSkillMap()
{
	AWHPlayerController* PC = Cast<AWHPlayerController>(GetOwner());
	if (IsValid(PC))
	{
		ISkillUsableInterface* SUI = Cast<ISkillUsableInterface>(PC->GetPawn());
		if (SUI != nullptr)
		{
			char Keys[] = { 'Q', 'W', 'E', 'R' };
			for (char Key : Keys)
			{
				UWHSkillBase* Skill = Cast<UWHSkillBase>(SUI->GetSkill(Key));
				if (IsValid(Skill))
				{
					SkillMap.Add(Key, { FTimerHandle(), Skill->CoolTime, false });
				}
			}
		}
	}
}

bool UWHCSkillHandler::CheckSkillCooldown(char Key)
{
	if (!SkillMap.Contains(Key))
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Key %c not found"), Key);
		return false;
	}
	
	FSkillData& Skill = SkillMap[Key];
	if (Skill.bIsOnCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Key %c is on cooldown"), Key);
		return false;
	}

	return true;
}

void UWHCSkillHandler::ApplyCooldown(char Key)
{
	if (!SkillMap.Contains(Key))
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Key %c not found"), Key);
		return;
	}

	FSkillData& Skill = SkillMap[Key];
	Skill.bIsOnCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(Skill.TimerHandle, FTimerDelegate::CreateUObject(this, &UWHCSkillHandler::ResetCooldown, Key), Skill.CoolTime, false);
}

void UWHCSkillHandler::ResetCooldown(char Key)
{
	if (SkillMap.Contains(Key))
	{
		SkillMap[Key].bIsOnCooldown = false;
		UE_LOG(LogTemp, Log, TEXT("Skill Key %c cooldown reset."), Key);
	}
}

