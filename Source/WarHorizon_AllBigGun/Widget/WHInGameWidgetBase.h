// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WHInGameWidgetBase.generated.h"

class UScrollBox;
class UEditableTextBox;


UCLASS()
class WARHORIZON_ALLBIGGUN_API UWHInGameWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct();

	UFUNCTION()
	void OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod);

	void AddChatMessage(FText AddMessage);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UScrollBox* ChatScroll;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UEditableTextBox* ChatText;


};
