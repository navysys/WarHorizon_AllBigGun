// Fill out your copyright notice in the Description page of Project Settings.


#include "WarHorizon_AllBigGun/Widget/WHLobbyWidgetBase.h"
#include "WarHorizon_AllBigGun/Controller/WHLobbyPlayerController.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"


void UWHLobbyWidgetBase::NativeConstruct()
{
	StartServerButton = Cast<UButton>(GetWidgetFromName(TEXT("StartServerButton")));
	if (StartServerButton)
	{
		StartServerButton->OnClicked.AddDynamic(this, &UWHLobbyWidgetBase::OnStartServerButtonClicked);
	}
	ConnectButton = Cast<UButton>(GetWidgetFromName(TEXT("ConnectButton")));
	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UWHLobbyWidgetBase::OnConnectButtonClicked);
	}
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerIP")));
}

void UWHLobbyWidgetBase::OnStartServerButtonClicked()
{
	// 데디케이트 서버 전환 시 삭제 될 함수
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("InGame"), true, TEXT("listen"));
}

void UWHLobbyWidgetBase::OnConnectButtonClicked()
{
	AWHLobbyPlayerController* LobbyPC = Cast<AWHLobbyPlayerController>(GetOwningPlayer());
	if (LobbyPC)
	{
		LobbyPC->TestJoinServer(FName(ServerIP->GetText().ToString()));
	}
}
