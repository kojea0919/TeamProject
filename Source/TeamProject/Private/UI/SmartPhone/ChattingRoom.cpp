// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/ChattingRoom.h"

void UChattingRoom::Init(class USmartPhone* Target)
{
	if (nullptr != Target)
		SmartPhone = Target; 
}
