#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Game/WHCustomStructs.h"
#include "Widget/WHInGameWidgetBase.h"
#include "WHPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class IBattleShipInterface;


UENUM()
enum class EControllerMappingType : uint8
{
	Default,
	WaitingAttack
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

private:
	IBattleShipInterface* BattleShipPawn;

protected:
	EControllerMappingType CurrentControllerMappingType;

	void ChangeWaitingAttackMappingContext();

	// IM
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> WaitingAttackContext;

	// IA
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveOrTargetingAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> RapidAttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> TargetAttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> SpinTurretAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AccelerationAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> DecelerationAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> ChangeContextAction;


	// 
	void MoveOrTargeting(const FInputActionValue& Value);

	void RapidAttack(const FInputActionValue& Value);

	void TargetAttack(const FInputActionValue& Value);

	void SpinTurret(const FInputActionValue& Value);

	void Acceleration(const FInputActionValue& Value);

	void Deceleration(const FInputActionValue& Value);

	void ChangeContext(const FInputActionValue& Value);

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
