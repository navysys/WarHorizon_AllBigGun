// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WHCTurretFSM.h"

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

	// ...
	
}


// Called every frame
void UWHCTurretFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (CurrentState)
	{
	case ETurretState::Idle:
		Idle(DeltaTime);
		break;
	case ETurretState::Turn:
		Turn(DeltaTime);
		break;
	case ETurretState::Stop:
		Stop(DeltaTime);
		break;
	case ETurretState::Ready:
		Ready(DeltaTime);
		break;
	case ETurretState::Fire:
		Fire(DeltaTime);
		break;
	case ETurretState::Invalid:
		UE_LOG(LogTemp, Warning, TEXT("Turret State Invalid"));
		break;
	default:
		break;
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
		CurrentState = ETurretState::Turn;
		CommandState = ETurretState::Idle;
	}
}

void UWHCTurretFSM::Turn(float DeltaTime)
{
	if (CommandState == ETurretState::Stop)
	{
		CurrentState = ETurretState::Stop;
		CommandState = ETurretState::Idle;
	}
	//회전 로직


	//회전 종료
	//if (Angle == GoalAngle)
	//{
	//	if (CommandState == ETurretState::Ready)
	//	{
	//		CurrentState = ETurretState::Ready;
	//	}
	//	else if (CommandState == ETurretState::Idle)
	//	{
	//		CurrentState = ETurretState::Idle;
	//	}
	//}
}

void UWHCTurretFSM::Stop(float DeltaTime)
{
	// GoalAngle 같은 수치 초기화 진행해야 함
	CurrentState = ETurretState::Idle;
}

void UWHCTurretFSM::Ready(float DeltaTime)
{
	// Fire 명령이 떨어지면 사격
	// 기본적으로 플레이어 조작으로는 Ready 까지만 가능
	// TurretHandler 컴포넌트에서 모든 포탑이 준비가 되었을 때 or 준비 된 포탑만이라도 공격할 때 Fire 상태 넘어가도록
	// 부포의 경우 타겟이 존재하면 쿨타임마다 Fire 로 넘어가도록 설정
}

void UWHCTurretFSM::Fire(float DeltaTime)
{
}

