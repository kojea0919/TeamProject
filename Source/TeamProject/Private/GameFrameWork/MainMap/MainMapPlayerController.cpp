// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "UI/MainHUD/PlayerMainHUD.h"

void AMainMapPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		InitInputMode();
		InitHUD();
	}
}

void AMainMapPlayerController::UpdateRemainTime(int Second)
{
	if (PlayerMainHUD)
	{
		PlayerMainHUD->UpdateRemainTime(Second);
	}
}

void AMainMapPlayerController::InitInputMode()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AMainMapPlayerController::InitHUD()
{
	if (PlayerMainHUD == nullptr && nullptr != PlayerMainHUDWidgetClass)
	{
		PlayerMainHUD = CreateWidget<UPlayerMainHUD>(this, PlayerMainHUDWidgetClass);
		if (PlayerMainHUD)
		{
			PlayerMainHUD->AddToViewport();			
		}
	}
}
