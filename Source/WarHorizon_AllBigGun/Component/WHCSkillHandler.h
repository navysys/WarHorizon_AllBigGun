#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WHCSkillHandler.generated.h"


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

public:
	//UPROPERTY(EditAnywhere, Category = WHSkillHandler)
	//TObjectPtr<UWHSkillBase> SkillQ;
	//UPROPERTY(EditAnywhere, Category = WHSkillHandler)
	//TObjectPtr<UWHSkillBase> SkillW;
	//UPROPERTY(EditAnywhere, Category = WHSkillHandler)
	//TObjectPtr<UWHSkillBase> SkillE;
	//UPROPERTY(EditAnywhere, Category = WHSkillHandler)
	//TObjectPtr<UWHSkillBase> SkillR;
};
