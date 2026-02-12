#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Game/WHCustomStructs.h"
#include "WHPlayerController.generated.h"

class UWHCSkillHandler;
class UInputMappingContext;
class UInputAction;
class IBattleShipInterface;
class UWHInGameWidgetBase;
class AFOW_ClientManager;


UENUM()
enum class EControllerMappingType : uint8
{
	Default,
	SelectTarget
};


UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWHPlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetupInputComponent() override;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
	TSubclassOf<AFOW_ClientManager> FogOfWarClientManagerClass;
	UPROPERTY(BlueprintReadOnly, Category = "Fog Of War")
	AFOW_ClientManager* FogOfWarClientManager;

	APawn* BattleShipPawn;
	char ActionKey;
	bool bIsAiming = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWHCSkillHandler> SkillHandlerComp;

protected:
	EControllerMappingType CurrentControllerMappingType;

	void ChangeSelectTargetMappingContext();

	// IM
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> SelectTargetContext;

	// IA
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveOrTargetingAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> SetFocusTargetAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> UseSkillAction_1;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> UseSkillAction_2;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> UseSkillAction_3;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> UseSkillAction_4;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> TargetAttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> SpinTurretAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AccelerationAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> DecelerationAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AimingFocusTargetAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AimingSkillAction_1;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AimingSkillAction_2;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AimingSkillAction_3;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AimingSkillAction_4;


	// 
	void MoveOrTargeting(const FInputActionValue& Value);

	void TargetAttack(const FInputActionValue& Value);

	void SpinTurret(const FInputActionValue& Value);

	void Acceleration(const FInputActionValue& Value);

	void Deceleration(const FInputActionValue& Value);

	void AimingSkill_1(const FInputActionValue& Value);
	void AimingSkill_2(const FInputActionValue& Value);
	void AimingSkill_3(const FInputActionValue& Value);
	void AimingSkill_4(const FInputActionValue& Value);

	void UseSkill_1(const FInputActionValue& Value);
	void UseSkill_2(const FInputActionValue& Value);
	void UseSkill_3(const FInputActionValue& Value);
	void UseSkill_4(const FInputActionValue& Value);

	void AimingFocusTarget(const FInputActionValue& Value);
	void SetFocusTarget(const FInputActionValue& Value);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWHInGameWidgetBase> InGameWidget;

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SendMessage(const FText& Messsage);
	bool C2S_SendMessage_Validate(const FText& Messsage);
	void C2S_SendMessage_Implementation(const FText& Message);

	UFUNCTION(Client, UnReliable)
	void S2C_AddMessage(const FText& Messsage);
	void S2C_AddMessage_Implementation(const FText& Messsage);


};
