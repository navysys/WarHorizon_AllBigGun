// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WHClientPlayerController.generated.h"


class UWHClientWidgetBase;


UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHClientPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWHClientWidgetBase> ClientWidget;


};
