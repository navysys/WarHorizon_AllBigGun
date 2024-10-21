
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
}

void AWHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//나중에 서버로 바꾸면 빙의 할 때 설정하도록 변경
	BattleShipPawn = Cast<IBattleShipInterface>(GetPawn());

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(DefaultContext, 1);
	}

	FSoftClassPath InGameWidgetClassPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_Main.WBP_Main_C'"));

	if (IsLocalPlayerController())
	{
		UClass* WidgetClass = InGameWidgetClassPath.TryLoadClass<UWHInGameWidgetBase>();
		if (IsValid(WidgetClass))
		{
			InGameWidget = CreateWidget<UWHInGameWidgetBase>(this, WidgetClass);
			if (IsValid(InGameWidget))
			{
				InGameWidget->AddToViewport();
			}
		}
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
		EnhancedInputComponent->BindAction(MoveOrTargetingAction, ETriggerEvent::Triggered, this, &AWHPlayerController::MoveOrTargeting);
		EnhancedInputComponent->BindAction(RapidAttackAction, ETriggerEvent::Triggered, this, &AWHPlayerController::RapidAttack);
		EnhancedInputComponent->BindAction(TargetAttackAction, ETriggerEvent::Triggered, this, &AWHPlayerController::TargetAttack);
		EnhancedInputComponent->BindAction(SpinTurretAction, ETriggerEvent::Triggered, this, &AWHPlayerController::SpinTurret);
		EnhancedInputComponent->BindAction(AccelerationAction, ETriggerEvent::Triggered, this, &AWHPlayerController::Acceleration);
		EnhancedInputComponent->BindAction(DecelerationAction, ETriggerEvent::Triggered, this, &AWHPlayerController::Deceleration);
		EnhancedInputComponent->BindAction(ChangeContextAction, ETriggerEvent::Triggered, this, &AWHPlayerController::ChangeContext);
	}
}

// 나중에 비행기를 컨트롤 하는 상태를 위해 조작을 제한하기 위함
void AWHPlayerController::ChangeWaitingAttackMappingContext()
{

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (CurrentControllerMappingType == EControllerMappingType::Default)
		{
			CurrentControllerMappingType = EControllerMappingType::WaitingAttack;
			SubSystem->AddMappingContext(WaitingAttackContext, 2);
		}
		else if (CurrentControllerMappingType == EControllerMappingType::WaitingAttack)
		{
			CurrentControllerMappingType = EControllerMappingType::Default;
			SubSystem->RemoveMappingContext(WaitingAttackContext);
		}
	}
}

void AWHPlayerController::MoveOrTargeting(const FInputActionValue& Value)
{
	if ( BattleShipPawn != nullptr)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// 이동에 대한 명령
			if (Hit.GetActor()->ActorHasTag(TEXT("Plane")))
			{
				BattleShipPawn->SpinBattleShip(Hit.Location);

				DrawDebugLine(GetWorld(), FVector(Hit.Location.X, Hit.Location.Y, 30000.0f), Hit.Location, FColor::Green, false, 3, 0, 200);
			}
			// 타겟 지정
			else if (Hit.GetActor()->ActorHasTag(TEXT("BattleShip")))
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Called"));
				AActor* TargetShip = Hit.GetActor();
				DrawDebugCircle(GetWorld(), FVector(TargetShip->GetActorLocation().X, TargetShip->GetActorLocation().Y, 800.0f), 8000.0f, 100, FColor::White, true, -1.f, 0, 50, FVector(1, 0, 0), FVector(0, 1, 0), true);
				BattleShipPawn->SpinTurrets(TargetShip);
				// 타겟이 되면 포탑이 이동속도에 비례해서 위치를 보정하고 서브 터렛이 해당 타겟을 조준하도록 해야함
			}
		}
	}
}

void AWHPlayerController::RapidAttack(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		BattleShipPawn->RapidAttack();

		// 디폴트 상태로 전환
		if (CurrentControllerMappingType == EControllerMappingType::WaitingAttack)
		{
			ChangeWaitingAttackMappingContext();
		}
	}

}

void AWHPlayerController::TargetAttack(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		// A 누른 뒤 함선을 클릭 했을 경우
		if (Hit.bBlockingHit)
		{
			if (Hit.GetActor()->ActorHasTag(TEXT("BattleShip")))
			{
				BattleShipPawn->SpinTurrets(Hit.GetActor());
				BattleShipPawn->NormalAttack();
			}
			else if (Hit.GetActor()->ActorHasTag(TEXT("Plane")))
			{
				// A 누른 뒤 바닥을 왼클릭 했을 경우
				BattleShipPawn->SpinTurrets(Hit.Location);
				BattleShipPawn->NormalAttack();
			}
			// 디폴트 상태로 전환
			if (CurrentControllerMappingType == EControllerMappingType::WaitingAttack)
			{
				ChangeWaitingAttackMappingContext();
			}
		}

	}
}

void AWHPlayerController::SpinTurret(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		
		if (Hit.bBlockingHit)
		{
			BattleShipPawn->SpinTurrets(Hit.Location);
			DrawDebugLine(GetWorld(), FVector(Hit.Location.X, Hit.Location.Y, 30000.0f), Hit.Location, FColor::Red, false, 3, 0, 200);
		}
	}
}

void AWHPlayerController::Acceleration(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		BattleShipPawn->IncreaseMoveSpeed();
	}
}

void AWHPlayerController::Deceleration(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		BattleShipPawn->DecreaseMoveSpeed();
	}
}

void AWHPlayerController::ChangeContext(const FInputActionValue& Value)
{
	ChangeWaitingAttackMappingContext();
}

bool AWHPlayerController::C2S_SendMessage_Validate(const FText& Messsage)
{
	return true;
}

void AWHPlayerController::C2S_SendMessage_Implementation(const FText& Message)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		AWHPlayerController* PC = Cast<AWHPlayerController>(*Iter);
		if (IsValid(PC))
		{
			PC->S2C_AddMessage(Message);
		}
	}
}

void AWHPlayerController::S2C_AddMessage_Implementation(const FText& Messsage)
{
	InGameWidget->AddChatMessage(Messsage);
}
