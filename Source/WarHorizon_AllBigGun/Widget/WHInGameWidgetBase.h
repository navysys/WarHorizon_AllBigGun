// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WHInGameWidgetBase.generated.h"

class UScrollBox;
class UEditableTextBox;
class UImage;


UCLASS()
class WARHORIZON_ALLBIGGUN_API UWHInGameWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod);

	void AddChatMessage(FText AddMessage);

	void UpdateMapMaterial(UMaterialInstanceDynamic* NewMaterial);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UScrollBox> ChatScroll;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEditableTextBox> ChatText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UImage> MapMaterial;
};
