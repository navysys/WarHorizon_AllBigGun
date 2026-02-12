
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
			
			UWHSkillBase* Skill = SUI->GetSkill(Key);
			if (IsValid(Skill))
			{
				// 함선 상태 체크 (상태이상, 마나)
				
				
				// 쿨타임 체크
				if (!CheckSkillCooldown(Key))
				{
					UE_LOG(LogTemp, Warning, TEXT(" Skill Key %c is on cooldown"), Key);
					return;
				}

				if (Skill->Effect())
				{
					// 마나, 쿨타임 적용
					ApplyCooldown(Key);

					UE_LOG(LogTemp, Warning, TEXT("Use Skill END"));
				}

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
			char Keys[] = { '1', '2', '3', '4' };
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
	
	if (SkillMap.Find(Key)->bIsOnCooldown)
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

	FSkillData* Skill = SkillMap.Find(Key);
	UE_LOG(LogTemp, Warning, TEXT("Skill.CoolTime : %f"), Skill->CoolTime);
	Skill->bIsOnCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(Skill->TimerHandle, FTimerDelegate::CreateUObject(this, &UWHCSkillHandler::ResetCooldown, Key), Skill->CoolTime, false);
}

void UWHCSkillHandler::ResetCooldown(char Key)
{
	if (SkillMap.Contains(Key))
	{
		SkillMap.Find(Key)->bIsOnCooldown = false;
		UE_LOG(LogTemp, Log, TEXT("Skill Key %c cooldown reset."), Key);
	}
}

