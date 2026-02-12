// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WHCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AWHCameraPawn::AWHCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoveSpeed = FVector2D(12000, 8000);
	ScreenEdgePadding = FVector2D(70, 70);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetRelativeRotation(FRotator(-50.0f, 90.0f, 0.0f));
	SpringArmComp->TargetArmLength = 10000.0f;
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritYaw = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->SetupAttachment(RootComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void AWHCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<APlayerController>(GetController());
}

// Called every frame
void AWHCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanCameraMove && IsValid(PlayerController))
	{
		FVector2D CurrentMoveSpeed = CurrentInputMoveSpeed;
		FVector2D MousePosition;

		if (PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
		{
			if (CurrentInputMoveSpeed.X == 0 && CurrentInputMoveSpeed.Y == 0)
			{
				int32 ViewportWidth, ViewportHeight;
				PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);

				const float X = (MousePosition.X <= ScreenEdgePadding.X) | -(MousePosition.X >= ViewportWidth - ScreenEdgePadding.X);
				const float Y = (MousePosition.Y <= ScreenEdgePadding.Y) | -(MousePosition.Y >= ViewportHeight - ScreenEdgePadding.Y);

				CurrentMoveSpeed = FVector2D(X, Y);
			}
			
			FVector Forward = GetActorForwardVector() * CurrentMoveSpeed.X * MoveSpeed.X * DeltaTime;
			FVector Sideways = GetActorRightVector() * CurrentMoveSpeed.Y * MoveSpeed.Y * DeltaTime;

			FVector NextLocation = GetActorLocation() + Forward + Sideways;
			SetActorLocation(NextLocation);
		}
	}
}

// Called to bind functionality to input
void AWHCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

