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

void USmartPhone::ReceiveChatMessage(const FText& Text, const FString& String)
{
	
}

void USmartPhone::UpdateScreen() const
{
	if ( nullptr != W_ChattingRoomList && nullptr != W_TeamChattingRoom &&
		nullptr != W_AllChattingRoom)
	{
		switch (CurScreenState)
		{
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
