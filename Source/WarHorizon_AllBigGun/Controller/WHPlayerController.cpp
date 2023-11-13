// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WHPlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Actor.h"
#include "Interface/BattleShipInterface.h"
#include "DrawDebugHelpers.h"

AWHPlayerController::AWHPlayerController()
{
	bShowMouseCursor = true;
	bAutoManageActiveCameraTarget = true;

	CurrentControllerMappingType = EControllerMappingType::Default;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext>IM_DEFAULT(TEXT("/Game/Input/IM_Default"));
	if (IM_DEFAULT.Succeeded())
	{
		DefaultContext = IM_DEFAULT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext>IM_WaitingAttack(TEXT("/Game/Input/IM_WaitingAttack"));
	if (IM_WaitingAttack.Succeeded())
	{
		WaitingAttackContext = IM_WaitingAttack.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MoveOrAttack(TEXT("/Game/Input/IA_MoveOrAttack"));
	if (IA_MoveOrAttack.Succeeded())
	{
		MoveOrAttackAction = IA_MoveOrAttack.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Attack(TEXT("/Game/Input/IA_Attack"));
	if (IA_Attack.Succeeded())
	{
		AttackAction = IA_Attack.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Acceleration(TEXT("/Game/Input/IA_Acceleration"));
	if (IA_Acceleration.Succeeded())
	{
		AccelerationAction = IA_Acceleration.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Deceleration(TEXT("/Game/Input/IA_Deceleration"));
	if (IA_Deceleration.Succeeded())
	{
		DecelerationAction = IA_Deceleration.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ChangeContext(TEXT("/Game/Input/IA_ChangeContext"));
	if (IA_ChangeContext.Succeeded())
	{
		ChangeContextAction = IA_ChangeContext.Object;
	}
}

void AWHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PawnBattleShip = Cast<IBattleShipInterface>(GetPawn());
	if (nullptr == PawnBattleShip)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Controller Pawn"));
	}

	Possess(GetPawn());

	//if (PawnBattleShip)
	//{
	//	Possess(PawnBattleShip);
	//}

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(DefaultContext, 0);
	}
}

void AWHPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveOrAttackAction, ETriggerEvent::Triggered, this, &AWHPlayerController::MoveOrAttack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWHPlayerController::Attack);
		EnhancedInputComponent->BindAction(AccelerationAction, ETriggerEvent::Triggered, this, &AWHPlayerController::Acceleration);
		EnhancedInputComponent->BindAction(DecelerationAction, ETriggerEvent::Triggered, this, &AWHPlayerController::Deceleration);
		EnhancedInputComponent->BindAction(ChangeContextAction, ETriggerEvent::Triggered, this, &AWHPlayerController::ChangeContext);
	}
}

void AWHPlayerController::SetControllerMappingType(EControllerMappingType NewControllerMappingType = EControllerMappingType::Default)
{

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->ClearAllMappings();

		UInputMappingContext* NewMappingContext = DefaultContext;
		if (NewControllerMappingType == EControllerMappingType::Default)
		{
			NewMappingContext = DefaultContext;
		}
		else if (NewControllerMappingType == EControllerMappingType::WaitingAttack)
		{
			NewMappingContext = WaitingAttackContext;
		}

		SubSystem->AddMappingContext(NewMappingContext, 0);
		CurrentControllerMappingType = NewControllerMappingType;
	}
}

void AWHPlayerController::MoveOrAttack(const FInputActionValue& Value)
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit && Hit.GetActor()->ActorHasTag(TEXT("Plane")))
	{
		PawnBattleShip->CalculateRotationToHitPoint(Hit.Location);

		DrawDebugLine(GetWorld(), FVector(Hit.Location.X, Hit.Location.Y, 30000.0f), Hit.Location, FColor::Red, false, 3, 0, 200);
	}
	else if (Hit.bBlockingHit && Hit.GetActor()->ActorHasTag(TEXT("BattleShip")))
	{
		APawn* TargetShip = Cast<APawn>(Hit.GetActor());
		DrawDebugCircle(GetWorld(), TargetShip->GetActorLocation(), 8000.0f, 100, FColor::White, true, -1.f, 0, 50, FVector(1, 0, 0), FVector(0, 1, 0), true);
		PawnBattleShip->UserAttack();
	}
}

void AWHPlayerController::Attack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Called!"));

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit && Hit.GetActor()->ActorHasTag(TEXT("BattleShip")))
	{
		APawn* TargetShip = Cast<APawn>(Hit.GetActor());
		PawnBattleShip->UserAttack();

		// 디폴트 상태로 전환
		SetControllerMappingType();
	}
}

void AWHPlayerController::Acceleration(const FInputActionValue& Value)
{
	PawnBattleShip->IncreaseMoveSpeed();
}

void AWHPlayerController::Deceleration(const FInputActionValue& Value)
{
	PawnBattleShip->IncreaseMoveSpeed();
}

void AWHPlayerController::ChangeContext(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ChangeContext Called!"));

	if (CurrentControllerMappingType == EControllerMappingType::Default)
	{
		SetControllerMappingType(EControllerMappingType::WaitingAttack);
	}
	else if (CurrentControllerMappingType == EControllerMappingType::WaitingAttack)
	{
		SetControllerMappingType(EControllerMappingType::Default);
	}
}