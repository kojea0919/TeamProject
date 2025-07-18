// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/ChattingRoom.h"
#if WITH_EDITOR
#include "EditorCategoryUtils.h"
#endif
#include "Blueprint/WidgetTree.h"
#include "UI/SmartPhone/SmartPhone.h"
#include "UI/SmartPhone/SelfTalkingBubble.h"
#include "UI/SmartPhone/OtherTalkingBubble.h"
#include "UI/SmartPhone/Emoji.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/EditableText.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/WrapBox.h"
#include "GameFramework/GameStateBase.h"
#include "GameFrameWork/MainMap/MainMapPlayerState.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "ChatType/ChatType.h"
#include "Player/Character/PlayerState/STPlayerState.h"

void UChattingRoom::Init(class USmartPhone* Target)
{
	if (nullptr != Target)
		SmartPhone = Target;

	InitRoomType();
	InitEmojiList();
}

void UChattingRoom::Clear()
{
	if (Vb_TalkingBubble)
		Vb_TalkingBubble->ClearChildren();
}

void UChattingRoom::AddChatSelfMessage(const FChatType & ChatType)
{
	if (SelfTalkingBubbleClass && Vb_TalkingBubble)
	{
		USelfTalkingBubble * NewTalkingBubble = CreateWidget<USelfTalkingBubble>(this, SelfTalkingBubbleClass);
		switch (ChatType.MessageType)
		{
		case EChatMessageType::Text:
			NewTalkingBubble->SetText(ChatType.Text);
			break;
		case EChatMessageType::Emoji:
			NewTalkingBubble->SetImage(TargetMaterial(ChatType.Emoji));
			break;
		}
		
		AddTalkingBubble(NewTalkingBubble);
	}	
}

void UChattingRoom::AddChatOtherMessage(const FChatType & ChatType, const FString& NickName)
{
	if (OtherTalkingBubbleClass && Vb_TalkingBubble)
	{
		UOtherTalkingBubble * NewTalkingBubble = CreateWidget<UOtherTalkingBubble>(this, OtherTalkingBubbleClass);
		switch (ChatType.MessageType)
		{
		case EChatMessageType::Text:
			NewTalkingBubble->SetInputText(ChatType.Text);
			break;
		case EChatMessageType::Emoji:
			NewTalkingBubble->SetImage(TargetMaterial(ChatType.Emoji));
			break;
		}
		
		NewTalkingBubble->SetNickName(NickName);
		NewTalkingBubble->SetProfile(ChatType.IsTagger);
		AddTalkingBubble(NewTalkingBubble);
	}
}

void UChattingRoom::FocusOn()
{
	if (Etb_InputText)
		Etb_InputText->SetFocus();
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

	if (nullptr != Btn_EmoListOpenButton)
	{
		Btn_EmoListOpenButton->OnClicked.AddDynamic(this,&UChattingRoom::ClickedEmoListOpenButton);
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
		AMainMapPlayerController * PlayerController =  GetOwningPlayer<AMainMapPlayerController>();
		ASTPlayerState * PlayerState = GetOwningPlayerState<ASTPlayerState>();
		if (PlayerController && PlayerState)
		{
			FChatType NewChat;
			NewChat.MessageType = EChatMessageType::Text;
			NewChat.Text = Text;			
			NewChat.IsTagger = PlayerState->IsPlayerTargger(); 
			PlayerController->SendChatMessageServer(NewChat, RoomType);
		}
				
		if (Etb_InputText)
		{
			Etb_InputText->SetText(FText::FromString(TEXT("")));
			Etb_InputText->SetKeyboardFocus();
		}
	}
}

void UChattingRoom::ClickedEmoListOpenButton()
{
	if (Sb_EmoList)
	{
		if (Sb_EmoList->GetVisibility() == ESlateVisibility::Hidden)
		{
			Sb_EmoList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			Sb_EmoList->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UChattingRoom::AddTalkingBubble(UWidget * AddWidget)
{	
	if (Vb_TalkingBubble)
	{
		Vb_TalkingBubble->AddChildToVerticalBox(AddWidget);
	}

	if (Scb_MsgScroll)
	{
		Scb_MsgScroll->ScrollToEnd();
	}
}

void UChattingRoom::InitRoomType()
{
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

void UChattingRoom::InitEmojiList()
{
	if (WB_EmojiList)
	{
		int32 Num = WB_EmojiList->GetChildrenCount();
		for (int32 Idx = 0 ; Idx < Num; ++Idx)
		{
			if (UEmoji * CurEmoji = Cast<UEmoji>(WB_EmojiList->GetChildAt(Idx)))
			{
				CurEmoji->OnAddTalkingBubble.BindLambda([this](EEmojiType EmojiType)
				{					
					//람다함수 본문
					//---------------------------------------------------------------------------------
					FChatType NewChat;
					NewChat.MessageType = EChatMessageType::Emoji;
					NewChat.Emoji = EmojiType;

					if (ASTPlayerState * PlayerState = GetOwningPlayerState<ASTPlayerState>())
						NewChat.IsTagger = PlayerState->IsPlayerTargger();

					if (AMainMapPlayerController * PlayerController =  GetOwningPlayer<AMainMapPlayerController>())
					{
						PlayerController->SendChatMessageServer(NewChat, RoomType);
					}
					//---------------------------------------------------------------------------------
				});
			}
		}
	}
}

UMaterialInstanceDynamic* UChattingRoom::TargetMaterial(EEmojiType EmojiType)
{
	int32 Num = WB_EmojiList->GetChildrenCount();	
	for (int32 Idx = 0 ; Idx < Num; ++Idx)
	{
		if (UEmoji * CurEmoji = Cast<UEmoji>(WB_EmojiList->GetChildAt(Idx)))
		{
			if (CurEmoji && CurEmoji->GetEmojiType() == EmojiType)
			{				
				return CurEmoji->GetCopyMaterialInstance(); 
			}
		}
	}

	return nullptr;
}
 