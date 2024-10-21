
#include "Widget/WHInGameWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Controller/WHPlayerController.h"

void UWHInGameWidgetBase::NativeConstruct()
{
	ChatScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatScroll")));
	ChatText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatText")));
	if (ChatText)
	{
		ChatText->OnTextCommitted.AddDynamic(this, &UWHInGameWidgetBase::OnCommittedText);
	}
}

void UWHInGameWidgetBase::OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod)
{
	UE_LOG(LogTemp, Warning, TEXT("On CommittedText Called"));
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
	{
		AWHPlayerController* PC = Cast<AWHPlayerController>(GetOwningPlayer());
		if (IsValid(PC))
		{
			PC->C2S_SendMessage(Text);
		}
		ChatText->SetText(FText::FromString(TEXT("")));
		break;
	}
	case ETextCommit::OnCleared:
		ChatText->SetUserFocus(GetOwningPlayer());
		break;
	}
}

void UWHInGameWidgetBase::AddChatMessage(FText AddMessage)
{
	UTextBlock* NewTextBox = NewObject<UTextBlock>(ChatScroll);
	if (IsValid(NewTextBox))
	{
		NewTextBox->SetText(AddMessage);
		FSlateFontInfo NewFont = NewTextBox->GetFont();
		NewFont.Size = 18.0f;
		NewTextBox->SetFont(NewFont);
		ChatScroll->AddChild(NewTextBox);
		ChatScroll->ScrollToEnd();
	}
}
