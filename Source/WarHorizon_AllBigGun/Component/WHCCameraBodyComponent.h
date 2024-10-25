#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WHCCameraBodyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARHORIZON_ALLBIGGUN_API UWHCCameraBodyComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWHCCameraBodyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ResetLocation();

public:
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY()
	FVector2D ScreenEdgePadding;

	UPROPERTY()
	FVector2D MoveSpeed;

	FVector2D CurrentInputMoveSpeed;

	UPROPERTY()
	bool bCanCameraMove = false;
};
