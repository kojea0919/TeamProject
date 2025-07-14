// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/SmartPhone.h"
#include "UI/SmartPhone/ChattingRoom.h"
#include "UI/SmartPhone/ChattingRoomList.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "UI/SmartPhone/InGameStart.h"

void USmartPhone::InitSmartPhone()
{
	CurScreenState = ESmartPhoneScreenState::RoomList;

	UpdateScreen();

	if (W_ChattingRoomList)
		W_ChattingRoomList->Init(this);

	if (W_AllChattingRoom)
		W_AllChattingRoom->Init(this);

	if (W_TeamChattingRoom)
		W_TeamChattingRoom->Init(this);
}

void USmartPhone::ChangeScreen(ESmartPhoneScreenState ScreenState)
{
	CurScreenState = ScreenState;
	UpdateScreen();
}

void USmartPhone::AddAllChatSelfMessage(const FChatType & ChatType)
{
	if (W_AllChattingRoom)
		W_AllChattingRoom->AddChatSelfMessage(ChatType);
}

void USmartPhone::AddAllChatOtherMessage(const FChatType & ChatType, const FString& NickName)
{
	if (W_AllChattingRoom)
	{
		W_AllChattingRoom->AddChatOtherMessage(ChatType,NickName);
	}

	//채팅방에 들어가있지 않은 경우 정보 업데이트
	if (CurScreenState != ESmartPhoneScreenState::AllChat)
	{
		W_ChattingRoomList->UpdateAllChatInfo(ChatType);
	}
}

void USmartPhone::AddTeamChatSelfMessage(const FChatType & ChatType)
{
	if (W_TeamChattingRoom)
	{
		W_TeamChattingRoom->AddChatSelfMessage(ChatType);
	}
}

void USmartPhone::AddTeamChatOtherMessage(const FChatType & ChatType, const FString& NickName)
{
	if (W_TeamChattingRoom)
	{
		W_TeamChattingRoom->AddChatOtherMessage(ChatType,NickName);
	}

	//채팅방에 들어가있지 않은 경우 정보 업데이트
	if (CurScreenState != ESmartPhoneScreenState::TeamChat)
	{
		W_ChattingRoomList->UpdateTeamChatInfo(ChatType);
	}
}

void USmartPhone::Clear()
{
	if (W_ChattingRoomList)
		W_ChattingRoomList->Clear();

	if (W_AllChattingRoom)
		W_AllChattingRoom->Clear();

	if (W_TeamChattingRoom)
		W_TeamChattingRoom->Clear();
}

void USmartPhone::UpdateScreen() const
{
	if ( nullptr != W_ChattingRoomList && nullptr != W_TeamChattingRoom &&
		nullptr != W_AllChattingRoom)
	{
		switch (CurScreenState)
		{
		case ESmartPhoneScreenState::None:
			break;
		case ESmartPhoneScreenState::RoomList:
			{
				W_ChattingRoomList->SetVisibility(ESlateVisibility::Visible);
				W_AllChattingRoom->SetVisibility(ESlateVisibility::Hidden);
				W_TeamChattingRoom->SetVisibility(ESlateVisibility::Hidden);
			}
			break;
		case ESmartPhoneScreenState::AllChat:
			{
				W_ChattingRoomList->SetVisibility(ESlateVisibility::Hidden);
				W_AllChattingRoom->SetVisibility(ESlateVisibility::Visible);
				W_AllChattingRoom->FocusOn();
				W_TeamChattingRoom->SetVisibility(ESlateVisibility::Hidden);
			}
			break;
		case ESmartPhoneScreenState::TeamChat:
			{
				W_ChattingRoomList->SetVisibility(ESlateVisibility::Hidden);
				W_AllChattingRoom->SetVisibility(ESlateVisibility::Hidden);
				W_TeamChattingRoom->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		default:
			break;
		}	
		
	}
}
