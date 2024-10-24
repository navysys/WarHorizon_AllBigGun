// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WHClientWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Controller/WHClientPlayerController.h"
#include "Kismet/GameplayStatics.h"


void UWHClientWidgetBase::NativeConstruct()
{
	StartServerButton = Cast<UButton>(GetWidgetFromName(TEXT("StartServerButton")));
	if (StartServerButton)
	{
		StartServerButton->OnClicked.AddDynamic(this, &UWHClientWidgetBase::OnStartServerButtonClicked);
	}
	ConnectButton = Cast<UButton>(GetWidgetFromName(TEXT("ConnectButton")));
	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UWHClientWidgetBase::OnConnectButtonClicked);
	}
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerIP")));
}

void UWHClientWidgetBase::OnStartServerButtonClicked()
{
	// 데디케이트 서버 전환 시 삭제 될 함수
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("InGame"), true, TEXT("listen"));
}

void UWHClientWidgetBase::OnConnectButtonClicked()
{
	AWHClientPlayerController* ClientPC = Cast<AWHClientPlayerController>(GetOwningPlayer());
	if (ClientPC)
	{
		ClientPC->TestJoinServer(FName(ServerIP->GetText().ToString()));
	}
}
