
#include "Component/WHCSkillHandler.h"


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

void UWHCSkillHandler::InitSkillHandlerComponent(TSoftObjectPtr<UObject> SkillQPtr, TSoftObjectPtr<UObject> SkillWPtr, TSoftObjectPtr<UObject> SkillEPtr, TSoftObjectPtr<UObject> SkillRPtr)
{
	if (SkillQPtr != nullptr)
	{
		UClass* AssetQClass = Cast<UClass>(SkillQPtr.LoadSynchronous());
		UWHSkillBase* SkillQInstance = NewObject<UWHSkillBase>(GetWorld(), AssetQClass);
		SkillQ = SkillQInstance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Handler -> InitSkillHandlerComponent : 스킬 Q NullPtr"));
	}

	if (SkillWPtr != nullptr)
	{
		UClass* AssetWClass = Cast<UClass>(SkillWPtr.LoadSynchronous());
		UWHSkillBase* SkillWInstance = NewObject<UWHSkillBase>(GetWorld(), AssetWClass);
		SkillW = SkillWInstance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Handler -> InitSkillHandlerComponent : 스킬 W NullPtr"));
	}

	if (SkillEPtr != nullptr)
	{
		UClass* AssetEClass = Cast<UClass>(SkillEPtr.LoadSynchronous());
		UWHSkillBase* SkillEInstance = NewObject<UWHSkillBase>(GetWorld(), AssetEClass);
		SkillE = SkillEInstance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Handler -> InitSkillHandlerComponent : 스킬 E NullPtr"));
	}

	if (SkillRPtr != nullptr)
	{
		UClass* AssetRClass = Cast<UClass>(SkillRPtr.LoadSynchronous());
		UWHSkillBase* SkillRInstance = NewObject<UWHSkillBase>(GetWorld(), AssetRClass);
		SkillR = SkillRInstance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Handler -> InitSkillHandlerComponent : 스킬 R NullPtr"));
	}
}

void UWHCSkillHandler::UseSkill(char Key)
{
	if (Key == 'Q')
	{
		SkillQ->Effect();
	}
	else if (Key == 'W')
	{
		SkillW->Effect();
	}
	else if (Key == 'E')
	{
		SkillE->Effect();
	}
	else if (Key == 'R')
	{
		SkillR->Effect();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Handler -> UseSkill : 알 수 없는 키값"));
	}
}

