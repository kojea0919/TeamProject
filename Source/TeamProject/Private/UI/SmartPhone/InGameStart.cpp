// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/InGameStart.h"
#include "Components/Button.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"

void UInGameStart::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_GameStart)
		Btn_GameStart->OnClicked.AddDynamic(this,&UInGameStart::StartGame);
		
}

void UInGameStart::StartGame()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			GameMode->GameStart();
		}
	}
}
