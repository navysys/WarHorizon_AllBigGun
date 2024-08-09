// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WHPlayerController.h"
#include "Game/WHCameraPawn.h"
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

	// 임시 이름 나중에 서버에서 들고오기
	BattleShipData.BattleShipName = TEXT("Yamato");

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

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MoveOrTargeting(TEXT("/Game/Input/IA_MoveOrTargeting"));
	if (IA_MoveOrTargeting.Succeeded())
	{
		MoveOrTargetingAction = IA_MoveOrTargeting.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_FastFire(TEXT("/Game/Input/IA_FastFire"));
	if (IA_FastFire.Succeeded())
	{
		FastFireAction = IA_FastFire.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Attack(TEXT("/Game/Input/IA_Attack"));
	if (IA_Attack.Succeeded())
	{
		AttackAction = IA_Attack.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_SpinTurret(TEXT("/Game/Input/IA_SpinTurret"));
	if (IA_SpinTurret.Succeeded())
	{
		SpinTurretAction = IA_SpinTurret.Object;
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

	CameraPawn = Cast<AWHCameraPawn>(GetPawn());
	//if (CameraPawn != nullptr)
	//{
	//	BattleShipPawn = Cast<IBattleShipInterface>(CameraPawn->GetPlayerBattleShip());
	//	if (BattleShipPawn == nullptr)
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Invalid Player BattleShip Pawn"));
	//	}
	//}
	
	Possess(GetPawn());

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
		EnhancedInputComponent->BindAction(MoveOrTargetingAction, ETriggerEvent::Triggered, this, &AWHPlayerController::MoveOrTargeting);
		EnhancedInputComponent->BindAction(FastFireAction, ETriggerEvent::Triggered, this, &AWHPlayerController::FastFire);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWHPlayerController::Attack);
		EnhancedInputComponent->BindAction(SpinTurretAction, ETriggerEvent::Triggered, this, &AWHPlayerController::SpinTurret);
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

void AWHPlayerController::MoveOrTargeting(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// 이동에 대한 명령
			if (Hit.GetActor()->ActorHasTag(TEXT("Plane")))
			{
				BattleShipPawn->CalculateRotationToHitPoint(Hit.Location);

				DrawDebugLine(GetWorld(), FVector(Hit.Location.X, Hit.Location.Y, 30000.0f), Hit.Location, FColor::Green, false, 3, 0, 200);
			}
			// 타겟 지정
			else if (Hit.GetActor()->ActorHasTag(TEXT("BattleShip")))
			{
				APawn* TargetShip = Cast<APawn>(Hit.GetActor());
				DrawDebugCircle(GetWorld(), FVector(TargetShip->GetActorLocation().X, TargetShip->GetActorLocation().Y, 100.0f), 8000.0f, 100, FColor::White, true, -1.f, 0, 50, FVector(1, 0, 0), FVector(0, 1, 0), true);
				BattleShipPawn->UserSpinTurretsToPawn(TargetShip);
				// 타겟이 되면 포탑이 이동속도에 비례해서 위치를 보정하고 서브 터렛이 해당 타겟을 조준하도록 해야함
			}
		}
	}
}

void AWHPlayerController::FastFire(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		BattleShipPawn->UserFastFire();

		// 디폴트 상태로 전환
		if (CurrentControllerMappingType == EControllerMappingType::WaitingAttack)
		{
			SetControllerMappingType(EControllerMappingType::Default);
		}
	}

}

void AWHPlayerController::Attack(const FInputActionValue& Value)
{
	if (BattleShipPawn != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Attack Called!"));

		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit && Hit.GetActor()->ActorHasTag(TEXT("BattleShip")))
		{
			APawn* TargetShip = Cast<APawn>(Hit.GetActor());
			BattleShipPawn->UserSpinTurretsToPawn(TargetShip);

			BattleShipPawn->UserAttack();

			// 디폴트 상태로 전환
			if (CurrentControllerMappingType == EControllerMappingType::WaitingAttack)
			{
				SetControllerMappingType(EControllerMappingType::Default);
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
			APawn* BattleShip = Cast<APawn>(BattleShipPawn);
			FVector Pos = FVector(BattleShip->GetActorLocation().X, BattleShip-> GetActorLocation().Y, 0);
			FVector HitPoint = FVector(Hit.Location.X, Hit.Location.Y, 0.0f);

			// 두 점 사이의 방향 벡터를 계산합니다
			FVector Direction = (HitPoint - Pos).GetSafeNormal();

			// 방향 벡터로부터 회전 값을 계산합니다
			float Angle = FRotationMatrix::MakeFromX(Direction).Rotator().Yaw;
			if (Angle < 0)
			{
				Angle += 360.0f;
			}

			// Hit 과 함선 사이의 거리
			float Distance = FVector::Distance(Pos, HitPoint);

			BattleShipPawn->UserSpinTurrets(Angle, Distance);
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

FBattleShipDataStruct AWHPlayerController::GetBattleShipData()
{
	return BattleShipData;
}

void AWHPlayerController::SetBattleShipPawn(IBattleShipInterface* BattleShipInterface)
{
	BattleShipPawn = BattleShipInterface;
}
