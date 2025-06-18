// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/ChattingRoomListSlot.h"
#include "UI/SmartPhone/SmartPhone.h"
#include "UI/SmartPhone/SmartPhoneEnumType.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

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
}
