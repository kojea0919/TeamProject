// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/ChattingRoom.h"
#include "UI/SmartPhone/SmartPhone.h"
#include "UI/SmartPhone/SelfTalkingBubble.h"
#include "UI/SmartPhone/OtherTalkingBubble.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "GameFramework/GameStateBase.h"
#include "GameFrameWork/MainMap/MainMapPlayerState.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"

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
		if (AMainMapPlayerController * PlayerController =  GetOwningPlayer<AMainMapPlayerController>())
			PlayerController->SendChatMessageServer(Text, RoomType);
		
		
		if (Etb_InputText)
		{
			Etb_InputText->SetText(FText::FromString(TEXT("")));
			Etb_InputText->SetKeyboardFocus();
		}
		
		//SendChatMessageServer(Text,CurPlayerState->StudentID);
		
		// if (SelfTalkingBubbleClass && Vb_TalkingBubble)
		// {
		// 	USelfTalkingBubble * NewTalkingBubble = CreateWidget<USelfTalkingBubble>(this, SelfTalkingBubbleClass);
		// 	NewTalkingBubble->SetText(Text);
		// 	Vb_TalkingBubble->AddChildToVerticalBox(NewTalkingBubble);
		// }	
		//
		//
		// if (Scb_MsgScroll)
		// 	Scb_MsgScroll->ScrollToEnd();

		//SendChatMessageServer(Text);
		//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red, TEXT("Good"));
	}
}

// void UChattingRoom::ReceiveChatMessage_Implementation(const FText& Text, const FString& SendPlayerNickName)
// {
// 	//if SelfMessage
// 	AMainMapPlayerState * CurPlayerState = Cast<AMainMapPlayerState>(GetOwningPlayerState());
// 	if (!IsValid(CurPlayerState)) return;
//
// 	if (CurPlayerState->PlayerNickName == SendPlayerNickName)
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,TEXT("Call"));
// 		USelfTalkingBubble * NewTalkingBubble = CreateWidget<USelfTalkingBubble>(this, SelfTalkingBubbleClass);
// 		NewTalkingBubble->SetText(Text);
// 		AddTalkingBubble(NewTalkingBubble);
// 	}
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,TEXT("OTher"));
// 		UOtherTalkingBubble * NewTalkingBubble = CreateWidget<UOtherTalkingBubble>(this, SelfTalkingBubbleClass);
// 		NewTalkingBubble->SetInputText(Text);
// 		NewTalkingBubble->SetNickName(SendPlayerNickName);
// 		AddTalkingBubble(NewTalkingBubble);
// 	}
//
// 	if (Scb_MsgScroll)
// 		Scb_MsgScroll->ScrollToEnd();
// }

void UChattingRoom::AddTalkingBubble(UUserWidget * AddWidget)
{	
	if (Vb_TalkingBubble)
	{
		Vb_TalkingBubble->AddChildToVerticalBox(AddWidget);
	}
}
 