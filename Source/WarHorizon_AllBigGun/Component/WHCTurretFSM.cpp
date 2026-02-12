// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WHCTurretFSM.h"
#include "Turret/WHTurretBase.h"

// Sets default values for this component's properties
UWHCTurretFSM::UWHCTurretFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWHCTurretFSM::BeginPlay()
{
	Super::BeginPlay();

	Turret = Cast<AWHTurretBase>(GetOwner());
	
}


// Called every frame
void UWHCTurretFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(Turret))
	{
		// 포신 상하 조정하는 함수는 상태 관계없이 계속 되도록 여기서 호출


		switch (CurrentState)
		{
		case ETurretState::Idle:
			Idle(DeltaTime);
			break;
		case ETurretState::Delay:
			Delay(DeltaTime);
			break;
		case ETurretState::Turn:
			Turn(DeltaTime);
			break;
		case ETurretState::Ready:
			Ready(DeltaTime);
			break;
		case ETurretState::Fire:
			Fire(DeltaTime);
			break;
		case ETurretState::AfterDelay:
			AfterDelay(DeltaTime);
			break;
		case ETurretState::Stop:
			Stop(DeltaTime);
			break;
		case ETurretState::Invalid:
			UE_LOG(LogTemp, Warning, TEXT("Turret State Invalid"));
			break;
		default:
			break;
		}
	}
}

void UWHCTurretFSM::SetCommandState(ETurretState State)
{
	CommandState = State;
}

ETurretState UWHCTurretFSM::GetCurrentState()
{
	return CurrentState;
}

void UWHCTurretFSM::Idle(float DeltaTime)
{
	if (CommandState == ETurretState::Turn)
	{
		BeforeActionTime = 0.0f;
		CurrentState = ETurretState::Delay;
	}

	//if (CommandState == ETurretState::Attack)
	//{
	//	BeforeActionTime = 0.0f;
	//	CurrentState = ETurretState::Delay;
	//}

	if (CommandState == ETurretState::AllClear)
	{
		BeforeActionTime = 0.0f;
		CurrentState = ETurretState::Ready;
	}

	if (CommandState == ETurretState::Stop)
	{
		CurrentState = ETurretState::Stop;
	}
}

void UWHCTurretFSM::Delay(float DeltaTime)
{
	BeforeActionTime += DeltaTime;

	if (BeforeActionTime > 0.5f)
	{
		if (CommandState == ETurretState::Turn)
		{
			BeforeActionTime = 0.0f;
			CurrentState = ETurretState::Turn;
			CommandState = ETurretState::Idle;
		}

		//if (CommandState == ETurretState::Attack)
		//{
		//	BeforeActionTime = 0.0f;
		//	CurrentState = ETurretState::Turn;
		//	CommandState = ETurretState::WaitingAttack;
		//}
	}
	if (CommandState == ETurretState::Stop)
	{
		CurrentState = ETurretState::Stop;
	}
}

void UWHCTurretFSM::Turn(float DeltaTime)
{
	if (CommandState == ETurretState::Stop)
	{
		CurrentState = ETurretState::Stop;
		return;
	}

	//if (CommandState == ETurretState::Attack)
	//{
	//	CurrentState = ETurretState::Idle;
	//	return;
	//}

	if (CommandState == ETurretState::AllClear)
	{
		Turret->SetIsLoaded(false);
		CommandState = ETurretState::Idle;
	}

	//회전 로직
	BeforeActionTime += DeltaTime;
	if (BeforeActionTime > TurnDelay)
	{
		BeforeActionTime -= TurnDelay;
		Turret->SpinToTargetAngle();
	}

	//회전 종료
	if (Turret->GetIsAimed())
	{
		//if (CommandState == ETurretState::Idle)
		//{
		//	BeforeActionTime = 0.0f;
		//	CurrentState = ETurretState::Idle;
		//}
		BeforeActionTime = 0.0f;
		CurrentState = ETurretState::Ready;
	}
}

void UWHCTurretFSM::Ready(float DeltaTime)
{
	if (CommandState == ETurretState::Stop)
	{
		CurrentState = ETurretState::Stop;
		return;
	}

	//Turret->CalculateAngleBetweenTarget();
	// 배가 회전하거나 타겟이 움직였을 경우
	if (!Turret->GetIsAimed())
	{
		CurrentState = ETurretState::Turn;
		return;
	}

	// AllClear 명령이 떨어지면 사격
	// TurretHandler 컴포넌트에서 포탑이 준비가 되었을 때 AllClear 상태 넘어가도록

	if (CommandState == ETurretState::AllClear)
	{
		CommandState = ETurretState::Idle;
		CurrentState = ETurretState::Fire;
	}
}

void UWHCTurretFSM::Fire(float DeltaTime)
{
	Turret->Fire();
	//쿨다운 상태로 전환 후 일정 시간 강제 대기
	CurrentState = ETurretState::AfterDelay;
}

void UWHCTurretFSM::AfterDelay(float DeltaTime)
{
	BeforeActionTime += DeltaTime;
	if (BeforeActionTime > 0.5f)
	{
		BeforeActionTime = 0.0f;
		if (Turret->GetIsAimed())
		{
			CurrentState = ETurretState::Ready;
		}
		else
		{
			CurrentState = ETurretState::Turn;
		}
	}
}

void UWHCTurretFSM::Stop(float DeltaTime)
{
	// GoalAngle 같은 수치 초기화 진행해야 함
	BeforeActionTime = 0.0f;
	CurrentState = ETurretState::Idle;
	CommandState = ETurretState::Idle;
}