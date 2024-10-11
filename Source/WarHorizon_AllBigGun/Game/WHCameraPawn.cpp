// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WHCameraPawn.h"
#include "Controller/WHPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "BattleShip/WHBattleShip.h"

// Sets default values
AWHCameraPawn::AWHCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoveSpeed = FVector2D(15000, 15000);
	ScreenEdgePadding = FVector2D(100, 100);

	BPBattleShip = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, TEXT("Blueprint'/Game/Blueprint/BP_BattleShip.BP_BattleShip'")));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetRelativeRotation(FRotator(-50.0f, 90.0f, 0.0f));
	SpringArmComp->TargetArmLength = 50000.0f;
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritYaw = false;
	SpringArmComp->bInheritRoll = false;
	//SpringArmComp->SetupAttachment(RootComponent);
	RootComponent = SpringArmComp;
	

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void AWHCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<AWHPlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		PlayerController->SetShowMouseCursor(true);
	}

	SpawnBattleShip();
}

// Called every frame
void AWHCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector2D CurrentMoveSpeed = CurrentInputMoveSpeed;
	FVector2D MousePosition;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	if (CurrentInputMoveSpeed.X == 0 && CurrentInputMoveSpeed.Y == 0)
	{
		int32 ViewportWidth, ViewportHeight;
		PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);

		const float X = -(MousePosition.X <= ScreenEdgePadding.X) | (MousePosition.X >= ViewportWidth - ScreenEdgePadding.X);
		const float Y = (MousePosition.Y <= ScreenEdgePadding.Y) | -(MousePosition.Y >= ViewportHeight - ScreenEdgePadding.Y);

		CurrentMoveSpeed = FVector2D(X, Y);
	}

	// Move Camera
	FVector ForwardVector = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0.0f);
	ForwardVector.Normalize();

	FVector Forward = ForwardVector * CurrentMoveSpeed.Y * MoveSpeed.Y * DeltaTime;
	FVector Sideways = GetActorRightVector() * CurrentMoveSpeed.X * MoveSpeed.X * DeltaTime;

	FVector NextLocation = GetActorLocation() + Forward + Sideways;
	SetActorLocation(NextLocation);
}

// Called to bind functionality to input
void AWHCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWHCameraPawn::SpawnBattleShip()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// 컨트롤러에서 정보를 불러와 스폰
	// 컨트롤러에서는 서버와 동기화 후 정보 전달 받아 생성하도록 해야함
	//PlayerBattleShip = GetWorld()->SpawnActor<AWHBattleShip>(BPBattleShip->GeneratedClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
	//PlayerController->SetBattleShipPawn(PlayerBattleShip);
}

AWHBattleShip* AWHCameraPawn::GetPlayerBattleShip()
{
	return PlayerBattleShip;
}

