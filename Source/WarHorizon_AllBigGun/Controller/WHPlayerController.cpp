
#include "Controller/WHPlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Actor.h"
#include "Interface/BattleShipInterface.h"
#include "DrawDebugHelpers.h"
#include "Widget/WHInGameWidgetBase.h"
#include "Component/WHCSkillHandler.h"


AWHPlayerController::AWHPlayerController()
{
	bShowMouseCursor = true;
	bAutoManageActiveCameraTarget = true;

	CurrentControllerMappingType = EControllerMappingType::Default;

	// 쿨타임을 서버에서 관리하기 위한 컴포넌트
	SkillHandlerComp = CreateDefaultSubobject<UWHCSkillHandler>(TEXT("SkillHandlerComp"));
}

void AWHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//나중에 서버로 바꾸면 빙의 할 때 설정하도록 변경
	BattleShipPawn =GetPawn();

	if (IsValid(SkillHandlerComp))
	{
		//SkillHandlerComp->InitSkillHandlerComponent(SkillPtrQ, SkillPtrW, SkillPtrE, SkillPtrR);
	}

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(DefaultContext, 1);
	}
	SetInputMode(FInputModeGameAndUI());

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
		EnhancedInputComponent->BindAction(SetFocusTargetAction, ETriggerEvent::Triggered, this, &AWHPlayerController::SetFocusTarget);
		EnhancedInputComponent->BindAction(UseSkillAction_Q, ETriggerEvent::Completed, this, &AWHPlayerController::UseSkill_Q);
		EnhancedInputComponent->BindAction(UseSkillAction_W, ETriggerEvent::Triggered, this, &AWHPlayerController::UseSkill_W);
		EnhancedInputComponent->BindAction(UseSkillAction_E, ETriggerEvent::Triggered, this, &AWHPlayerController::UseSkill_E);
		EnhancedInputComponent->BindAction(UseSkillAction_R, ETriggerEvent::Triggered, this, &AWHPlayerController::UseSkill_R);
		EnhancedInputComponent->BindAction(TargetAttackAction, ETriggerEvent::Triggered, this, &AWHPlayerController::TargetAttack);
		EnhancedInputComponent->BindAction(SpinTurretAction, ETriggerEvent::Started, this, &AWHPlayerController::SpinTurret);
		EnhancedInputComponent->BindAction(AccelerationAction, ETriggerEvent::Triggered, this, &AWHPlayerController::Acceleration);
		EnhancedInputComponent->BindAction(DecelerationAction, ETriggerEvent::Triggered, this, &AWHPlayerController::Deceleration);
		EnhancedInputComponent->BindAction(AimingFocusTargetAction, ETriggerEvent::Triggered, this, &AWHPlayerController::AimingFocusTarget);
		EnhancedInputComponent->BindAction(AimingSkillAction_Q, ETriggerEvent::Triggered, this, &AWHPlayerController::AimingSkill_Q);
		EnhancedInputComponent->BindAction(AimingSkillAction_W, ETriggerEvent::Triggered, this, &AWHPlayerController::AimingSkill_W);
		EnhancedInputComponent->BindAction(AimingSkillAction_E, ETriggerEvent::Triggered, this, &AWHPlayerController::AimingSkill_E);
		EnhancedInputComponent->BindAction(AimingSkillAction_R, ETriggerEvent::Triggered, this, &AWHPlayerController::AimingSkill_R);
	}
}

// 나중에 비행기를 컨트롤 하는 상태를 위해 조작을 제한하기 위함
void AWHPlayerController::ChangeSelectTargetMappingContext()
{

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (CurrentControllerMappingType == EControllerMappingType::Default)
		{
			CurrentControllerMappingType = EControllerMappingType::SelectTarget;
			SubSystem->AddMappingContext(SelectTargetContext, 2);
		}
		else if (CurrentControllerMappingType == EControllerMappingType::SelectTarget)
		{
			CurrentControllerMappingType = EControllerMappingType::Default;
			SubSystem->RemoveMappingContext(SelectTargetContext);
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
			IBattleShipInterface* BSI = Cast<IBattleShipInterface>(BattleShipPawn);
			// 이동에 대한 명령
			if (Hit.GetActor()->ActorHasTag(TEXT("Plane")))
			{
				if (BSI != nullptr)
				{
					BSI->SpinBattleShip(Hit.Location);
				}
				DrawDebugLine(GetWorld(), FVector(Hit.Location.X, Hit.Location.Y, 30000.0f), Hit.Location, FColor::Green, false, 3, 0, 200);
			}
			// 타겟 지정
			else if (Hit.GetActor()->ActorHasTag(TEXT("BattleShip")))
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Called"));
				AActor* TargetShip = Hit.GetActor();
				DrawDebugCircle(GetWorld(), FVector(TargetShip->GetActorLocation().X, TargetShip->GetActorLocation().Y, 800.0f), 8000.0f, 100, FColor::White, true, -1.f, 0, 50, FVector(1, 0, 0), FVector(0, 1, 0), true);
				if (BSI != nullptr)
				{
					BSI->SpinTurrets(TargetShip);
				}
				// 타겟이 되면 포탑이 이동속도에 비례해서 위치를 보정하고 서브 터렛이 해당 타겟을 조준하도록 해야함
			}
		}
	}
}

void AWHPlayerController::RapidAttack(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		IBattleShipInterface* BSI = Cast<IBattleShipInterface>(BattleShipPawn);
		if (BSI != nullptr)
		{
			BSI->RapidAttack();
		}
	}

}

void AWHPlayerController::TargetAttack(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		IBattleShipInterface* BSI = Cast<IBattleShipInterface>(BattleShipPawn);
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		// A 누른 뒤 함선을 클릭 했을 경우
		if (Hit.bBlockingHit)
		{
			if (Hit.GetActor()->ActorHasTag(TEXT("BattleShip")))
			{
				if (BSI != nullptr)
				{
					BSI->SpinTurrets(Hit.GetActor());
					BSI->NormalAttack();
				}

			}
			else if (Hit.GetActor()->ActorHasTag(TEXT("Plane")))
			{
				// A 누른 뒤 바닥을 왼클릭 했을 경우
				if (BSI != nullptr)
				{
					BSI->SpinTurrets(Hit.Location);
					BSI->NormalAttack();
				}

			}
		}

	}
}

void AWHPlayerController::SpinTurret(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("SpinTurret Called"));

	if (BattleShipPawn != nullptr)
	{
		IBattleShipInterface* BSI = Cast<IBattleShipInterface>(BattleShipPawn);
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		
		if (Hit.bBlockingHit)
		{
			if (BSI != nullptr)
			{
				BSI->SpinTurrets(Hit.Location);
				DrawDebugLine(GetWorld(), FVector(Hit.Location.X, Hit.Location.Y, 30000.0f), Hit.Location, FColor::Red, false, 3, 0, 200);
			}
		}
	}
}

void AWHPlayerController::Acceleration(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		IBattleShipInterface* BSI = Cast<IBattleShipInterface>(BattleShipPawn);
		if (BSI != nullptr)
		{
			BSI->IncreaseMoveSpeed();
		}
	}
}

void AWHPlayerController::Deceleration(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		IBattleShipInterface* BSI = Cast<IBattleShipInterface>(BattleShipPawn);
		if (BSI != nullptr)
		{
			BSI->DecreaseMoveSpeed();
		}
	}
}

void AWHPlayerController::AimingFocusTarget(const FInputActionValue& Value)
{
	ActionKey = 'A';
	// 여기서 사거리 UI 출력
}

void AWHPlayerController::AimingSkill_Q(const FInputActionValue& Value)
{
	// 일단 쿨타임인지 클라이언트 기준으로 검사


	UE_LOG(LogTemp, Warning, TEXT("Aim Skill Q"));
	ActionKey = 'Q';
	// 여기서 사거리 UI 출력 , 다른 키와 동시에 누른 경우에 이전에 활성화 되어 있던 UI 비활성화 후 UI 출력
	if (!bIsAiming)
	{
		bIsAiming = true;
	}
	else
	{
		// 기존 UI 끄고 해당 스킬의 UI 표시
	}
}

void AWHPlayerController::AimingSkill_W(const FInputActionValue& Value)
{
	ActionKey = 'W';
	// 여기서 사거리 UI 출력
}

void AWHPlayerController::AimingSkill_E(const FInputActionValue& Value)
{
	ActionKey = 'E';
	// 여기서 사거리 UI 출력
}

void AWHPlayerController::AimingSkill_R(const FInputActionValue& Value)
{
	ActionKey = 'R';
	// 여기서 사거리 UI 출력
}

void AWHPlayerController::UseSkill_Q(const FInputActionValue& Value)
{
	if (ActionKey == 'Q')
	{
		UE_LOG(LogTemp, Warning, TEXT("UseSkill_Q"));
		bIsAiming = false;
		SkillHandlerComp->UseSkill(ActionKey);
	}
}

void AWHPlayerController::UseSkill_W(const FInputActionValue& Value)
{
	if (ActionKey == 'W')
	{
		SkillHandlerComp->UseSkill(ActionKey);
	}
}

void AWHPlayerController::UseSkill_E(const FInputActionValue& Value)
{
	if (ActionKey == 'E')
	{
		SkillHandlerComp->UseSkill(ActionKey);
	}
}

void AWHPlayerController::UseSkill_R(const FInputActionValue& Value)
{
	if (ActionKey == 'R')
	{
		SkillHandlerComp->UseSkill(ActionKey);
	}
}

void AWHPlayerController::SetFocusTarget(const FInputActionValue& Value)
{

}

// false 이라면 서버 호출이 거절
bool AWHPlayerController::C2S_SendMessage_Validate(const FText& Messsage)
{
	return true;
}

// 서버에서만 실행 될 함수
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

// 서버에서 클라이언트로 호출
void AWHPlayerController::S2C_AddMessage_Implementation(const FText& Messsage)
{
	InGameWidget->AddChatMessage(Messsage);
}
