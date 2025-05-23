// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WHLoginGameMode.h"
#include "Kismet/GameplayStatics.h"

AWHLoginGameMode::AWHLoginGameMode()
{
	Http = &FHttpModule::Get();
}

void AWHLoginGameMode::BeginPlay()
{
	Super::BeginPlay();
	//HttpCall("localhost:8080", "GET");
}

void AWHLoginGameMode::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP request failed. URL: %s"), *Request->GetURL());
		return;
	}

	int32 StatusCode = Response->GetResponseCode();

	FString ResponseContent = Response->GetContentAsString();

	if (ResponseContent == TEXT("Pass"))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Client"));
	}
}

void AWHLoginGameMode::HttpCall(const FString& InURL, const FString& InVerb, FString Content)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AWHLoginGameMode::OnResponseReceived);

	Request->SetURL(InURL);
	Request->SetVerb(InVerb);
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString(Content);

	Request->ProcessRequest();
}
