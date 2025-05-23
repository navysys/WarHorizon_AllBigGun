
#include "Widget/WHInGameWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Controller/WHPlayerController.h"

void UWHInGameWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ChatScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatScroll")));
	ChatText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatTextBox")));
	if (ChatText)
	{
		// ChatText 의 델리게이트에 함수 추가
		ChatText->OnTextCommitted.AddDynamic(this, &UWHInGameWidgetBase::OnCommittedText);
	}
}

void UWHInGameWidgetBase::OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
	{
		AWHPlayerController* PC = Cast<AWHPlayerController>(GetOwningPlayer());
		if (IsValid(PC))
		{
			// 플레이어 컨트롤러의 함수 호출 -> 클라이언트에서 서버로 메세지 송신
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
