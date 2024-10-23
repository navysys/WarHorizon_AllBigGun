// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WHClientWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"


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
}

void UWHClientWidgetBase::OnConnectButtonClicked()
{
}
