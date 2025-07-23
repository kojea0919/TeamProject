// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/ChattingRoomList.h"

#include "Components/Image.h"
#include "UI/SmartPhone/ChattingRoomListSlot.h"
#include "Components/Slider.h"

void UChattingRoomList::Init(class USmartPhone* Target)
{
	if (nullptr != Target)
		SmartPhone = Target;

	W_AllChatRoomListSlot->Init(Target);
	W_TeamChatRoomListSlot->Init(Target);
}

void UChattingRoomList::Clear()
{
	W_AllChatRoomListSlot->RemoveChatInfo();
	W_TeamChatRoomListSlot->RemoveChatInfo();
}

void UChattingRoomList::UpdateAllChatInfo(const FChatType& Text)
{
	if (W_AllChatRoomListSlot)
		W_AllChatRoomListSlot->UpdateChatInfo(Text);
}

void UChattingRoomList::UpdateTeamChatInfo(const FChatType& Text)
{
	if (W_TeamChatRoomListSlot)
		W_TeamChatRoomListSlot->UpdateChatInfo(Text);
}

void UChattingRoomList::NativeConstruct()
{
	Super::NativeConstruct();

	if (S_MicVolumeSlide)
	{
		S_MicVolumeSlide->OnValueChanged.AddDynamic(this,&UChattingRoomList::ChangeMicVolume);
	}
}

void UChattingRoomList::ChangeMicVolume(float Value)
{
	if (nullptr == Img_MicOff || nullptr == Img_MicOn)
		return;

	
	if (Value == 0)
	{
		Img_MicOff->SetVisibility(ESlateVisibility::Visible);
		Img_MicOn->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Img_MicOn->SetVisibility(ESlateVisibility::Visible);
		Img_MicOff->SetVisibility(ESlateVisibility::Hidden);

		FString MicVolume = TEXT("VoiceInputGain ") + FString::FromInt(Value);
		GetOwningPlayer()->ConsoleCommand(MicVolume);
	}
}
