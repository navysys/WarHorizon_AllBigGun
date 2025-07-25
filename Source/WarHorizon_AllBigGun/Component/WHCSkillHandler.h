#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WHCSkillHandler.generated.h"


USTRUCT()
struct FSkillData
{
	GENERATED_BODY()

	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY()
	float CoolTime = 0.f;

	UPROPERTY()
	bool bIsOnCooldown = false;

	// 필요에 따라 스킬 클래스 포인터도 여기에 추가
	// TObjectPtr<UWHSkillBase> SkillInstance;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARHORIZON_ALLBIGGUN_API UWHCSkillHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWHCSkillHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UseSkill(char Key);

private:
	TMap<char, FSkillData> SkillMap;

	void InitSkillMap();
	bool CheckSkillCooldown(char Key);
	void ApplyCooldown(char Key);
	void ResetCooldown(char Key);
};
