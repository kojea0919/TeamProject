// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/ChattingRoomList.h"
#include "UI/SmartPhone/ChattingRoomListSlot.h"

void UChattingRoomList::Init(class USmartPhone* Target)
{
	if (nullptr != Target)
		SmartPhone = Target;

	W_AllChatRoomListSlot->Init(Target);
	W_TeamChatRoomListSlot->Init(Target);
}
