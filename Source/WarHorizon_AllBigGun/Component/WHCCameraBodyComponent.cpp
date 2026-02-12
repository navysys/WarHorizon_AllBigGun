// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WHCCameraBodyComponent.h"

// Sets default values for this component's properties
UWHCCameraBodyComponent::UWHCCameraBodyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MoveSpeed = FVector2D(15000, 10000);
	ScreenEdgePadding = FVector2D(100, 100);
}


// Called when the game starts
void UWHCCameraBodyComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (IsValid(OwnerPawn))
	{
		PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	}
}


// Called every frame
void UWHCCameraBodyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

			FVector Forward = GetForwardVector() * CurrentMoveSpeed.Y * MoveSpeed.Y * DeltaTime;
			FVector Sideways = -GetRightVector() * CurrentMoveSpeed.X * MoveSpeed.X * DeltaTime;

			FVector NextLocation = GetRelativeLocation() + Forward + Sideways;
			SetRelativeLocation(NextLocation);
		}
	}
}

void UWHCCameraBodyComponent::ResetLocation()
{
	SetRelativeLocation(FVector::Zero());
}

