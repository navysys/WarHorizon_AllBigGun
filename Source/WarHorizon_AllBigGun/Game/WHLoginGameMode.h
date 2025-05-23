// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "WHLoginGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WARHORIZON_ALLBIGGUN_API AWHLoginGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AWHLoginGameMode();
	virtual void BeginPlay() override;

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION()
	void HttpCall(const FString& InURL, const FString& InVerb, FString Content);

public:
	FHttpModule* Http;
};
