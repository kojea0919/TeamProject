// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/SmartPhone.h"
#include "UI/SmartPhone/ChattingRoom.h"
#include "UI/SmartPhone/ChattingRoomList.h"

void USmartPhone::InitSmartPhone()
{
	CurScreenState = ESmartPhoneScreenState::RoomList;

	UpdateScreen();

	W_ChattingRoomList->Init(this);
	W_AllChattingRoom->Init(this);
	W_TeamChattingRoom->Init(this);
}

void USmartPhone::ChangeScreen(ESmartPhoneScreenState ScreenState)
{
	CurScreenState = ScreenState;
	UpdateScreen();
}

void USmartPhone::AddAllChatSelfMessage(const FText& Text)
{
	if (W_AllChattingRoom)
		W_AllChattingRoom->AddChatSelfMessage(Text);
}

void USmartPhone::AddAllChatOtherMessage(const FText& Text, const FString& NickName)
{
	if (W_AllChattingRoom)
	{
		W_AllChattingRoom->AddChatOtherMessage(Text,NickName);
	}

	//채팅방에 들어가있지 않은 경우 정보 업데이트
	if (CurScreenState != ESmartPhoneScreenState::AllChat)
	{
		W_ChattingRoomList->UpdateAllChatInfo(Text);
	}
}

void USmartPhone::AddTeamChatSelfMessage(const FText& Text)
{
	if (W_TeamChattingRoom)
	{
		W_TeamChattingRoom->AddChatSelfMessage(Text);
	}
}

void USmartPhone::AddTeamChatOtherMessage(const FText& Text, const FString& NickName)
{
	if (W_TeamChattingRoom)
	{
		W_TeamChattingRoom->AddChatOtherMessage(Text,NickName);
	}

	//채팅방에 들어가있지 않은 경우 정보 업데이트
	if (CurScreenState != ESmartPhoneScreenState::TeamChat)
	{
		W_ChattingRoomList->UpdateTeamChatInfo(Text);
	}
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
