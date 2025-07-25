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
	//ȸ�� ����


	//ȸ�� ����
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
	// GoalAngle ���� ��ġ �ʱ�ȭ �����ؾ� ��
	CurrentState = ETurretState::Idle;
}

void UWHCTurretFSM::Ready(float DeltaTime)
{
	// Fire ����� �������� ���
	// �⺻������ �÷��̾� �������δ� Ready ������ ����
	// TurretHandler ������Ʈ���� ��� ��ž�� �غ� �Ǿ��� �� or �غ� �� ��ž���̶� ������ �� Fire ���� �Ѿ����
	// ������ ��� Ÿ���� �����ϸ� ��Ÿ�Ӹ��� Fire �� �Ѿ���� ����
}

void UWHCTurretFSM::Fire(float DeltaTime)
{
}

