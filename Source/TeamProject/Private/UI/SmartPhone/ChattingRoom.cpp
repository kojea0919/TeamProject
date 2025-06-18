// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/ChattingRoom.h"
#include "UI/SmartPhone/SmartPhone.h"
#include "UI/SmartPhone/SelfTalkingBubble.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/EditableText.h"
#include "GameFrameWork/MainMap/MainMapPlayerState.h"

void UChattingRoom::Init(class USmartPhone* Target)
{
	if (nullptr != Target)
		SmartPhone = Target;

	switch (RoomType)
	{
	case EChattingRoomType::AllChatRoom:
		Tb_RoomName->SetText(FText::FromString(TEXT("단톡방")));
		break;
	case EChattingRoomType::TeamChatRoom:
		{
			if (AMainMapPlayerState * PlayerState = Cast<AMainMapPlayerState>(GetOwningPlayerState()))
			{
				if (PlayerState->IsPlayerTargger())
					Tb_RoomName->SetText(FText::FromString(TEXT("술래방")));
				else
					Tb_RoomName->SetText(FText::FromString(TEXT("시민방")));
			}			
		}
		break;
	}
}

void UChattingRoom::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != Btn_TabletBack)
	{
		Btn_TabletBack->OnClicked.AddDynamic(this,&UChattingRoom::GoToChattingRoomList);
	}

	if (nullptr != B_InputTextBorder)
	{
		B_InputTextBorder->OnMouseButtonDownEvent.BindUFunction(this,TEXT("FocusInputEditTextBox")); 
	}

	if (nullptr != Etb_InputText)
	{
		Etb_InputText->OnTextCommitted.AddDynamic(this, &UChattingRoom::TextCommit);
	}
}

void UChattingRoom::GoToChattingRoomList()
{
	if (SmartPhone)
		SmartPhone->ChangeScreen(ESmartPhoneScreenState::RoomList);
}

void UChattingRoom::FocusInputEditTextBox()
{
	if (Etb_InputText)
		Etb_InputText->SetKeyboardFocus();
}

void UChattingRoom::TextCommit(const FText& Text, ETextCommit::Type Type)
{
	if (ETextCommit::OnEnter == Type)
	{
		// if (SelfTalkingBubbleClass && Vb_TalkingBubble)
		// {
		// 	USelfTalkingBubble * NewTalkingBubble = CreateWidget<USelfTalkingBubble>(this, SelfTalkingBubbleClass);
		// 	Vb_TalkingBubble->AddChildToVerticalBox(NewTalkingBubble);
		// 	UE_LOG(LogTemp,Warning,TEXT("FE"));
		// 	
		// }
		//
		 if (SelfTalkingBubbleClass)
		 	UE_LOG(LogTemp,Warning,TEXT("1"));

		if (!Vb_TalkingBubble)
			UE_LOG(LogTemp,Warning,TEXT("2"));

		if (Etb_InputText)
			Etb_InputText->SetText(FText::FromString(TEXT("")));
	}
}
