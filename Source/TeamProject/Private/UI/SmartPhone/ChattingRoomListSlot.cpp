// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/ChattingRoomListSlot.h"
#include "UI/SmartPhone/SmartPhone.h"
#include "UI/SmartPhone/SmartPhoneEnumType.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UChattingRoomListSlot::Init(USmartPhone * Target)
{
	if (nullptr != Target)
		SmartPhone = Target;

	if (Tb_ChatRoomName)
	{
		switch (ConnectChattingRoomType)
		{
		case EChattingRoomType::AllChatRoom:
			Tb_ChatRoomName->SetText(FText::FromString(TEXT("단톡방")));
			break;
		case EChattingRoomType::TeamChatRoom:
			Tb_ChatRoomName->SetText(FText::FromString(TEXT("팀방")));
			break;
		}
	}

	if (Tb_RecentMsg)
		Tb_RecentMsg->SetVisibility(ESlateVisibility::Hidden);

	if (Tb_MsgNum)
		Tb_MsgNum->SetVisibility(ESlateVisibility::Hidden);

	if (Img_MsgNumFrame)
		Img_MsgNumFrame->SetVisibility(ESlateVisibility::Hidden);

	UnreadMsgCount = 0;
}

void UChattingRoomListSlot::UpdateChatInfo(const FChatType& ChatType)
{
	if (Tb_RecentMsg)
	{
		Tb_RecentMsg->SetVisibility(ESlateVisibility::Visible);

		switch (ChatType.MessageType)
		{
		case EChatMessageType::Text:
		{
			Tb_RecentMsg->SetText(ChatType.Text);
		}
			break;
		case EChatMessageType::Emoji:
		{
			Tb_RecentMsg->SetText(FText::FromString("Emoji"));
		}
			break;
		}
		
	}

	if (Img_MsgNumFrame)
	{
		Img_MsgNumFrame->SetVisibility(ESlateVisibility::Visible);
	}

	if (Tb_MsgNum)
	{
		Tb_MsgNum->SetVisibility(ESlateVisibility::Visible);
		Tb_MsgNum->SetText(FText::AsNumber(++UnreadMsgCount));
	}		
}

void UChattingRoomListSlot::RemoveChatInfo()
{
	if (Tb_RecentMsg)
	{
		Tb_RecentMsg->SetVisibility(ESlateVisibility::Hidden);
		Tb_MsgNum->SetText(FText::FromString(TEXT("")));
	}

	if (Img_MsgNumFrame)
	{
		Img_MsgNumFrame->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Tb_MsgNum)
	{
		Tb_MsgNum->SetVisibility(ESlateVisibility::Hidden);
		Tb_MsgNum->SetText(FText::FromString(TEXT("")));
		UnreadMsgCount = 0;
	}		
}

void UChattingRoomListSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_ChattingRoom)
		Btn_ChattingRoom->OnClicked.AddDynamic(this,&UChattingRoomListSlot::EnterRoom);
}

void UChattingRoomListSlot::EnterRoom()
{
	if (nullptr == SmartPhone)
		return;
		
	switch(ConnectChattingRoomType)
	{
	case EChattingRoomType::AllChatRoom:
		SmartPhone->ChangeScreen(ESmartPhoneScreenState::AllChat);		
		break;
	case EChattingRoomType::TeamChatRoom:
		SmartPhone->ChangeScreen(ESmartPhoneScreenState::TeamChat);
		break;
	default:
		break;
	}

	RemoveChatInfo();
}
