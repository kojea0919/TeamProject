#include "GameFrameWork/StartMap/StartMapPlayerController.h"
#include "UI/StartMapUI/StartMapHUD.h"

void AStartMapPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitHUD();
	InitInputMode();
}

void AStartMapPlayerController::InitHUD()
{
	if (StartMapHUDClass && StartMapHUD == nullptr)
	{
		StartMapHUD = CreateWidget<UStartMapHUD>(this, StartMapHUDClass);
		if (StartMapHUD)
		{
			StartMapHUD->AddToViewport();
		}
	}
}

void AStartMapPlayerController::InitInputMode()
{
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode); 
	bShowMouseCursor = true; 
}
