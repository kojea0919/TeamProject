#include "GameFrameWork/StartMap/StartMapPlayerController.h"
#include "UI/StartMapUI/StartMapHUD.h"

void AStartMapPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitHUD();
	InitInputMode();
}

void AStartMapPlayerController::AddToSessionList(FBlueprintSessionResult SessionResult)
{
	if (StartMapHUD)
		StartMapHUD->AddToSessionList(SessionResult);
}

void AStartMapPlayerController::ClearSessionList()
{
	if (StartMapHUD)
		StartMapHUD->ClearSessionList();
}

void AStartMapPlayerController::SetVisibleSessionLoadImage(bool Visible)
{
	if (StartMapHUD)
		StartMapHUD->SetVisibleSessionLoadImage(Visible);
}

void AStartMapPlayerController::InitHUD()
{
	if (StartMapHUDClass && StartMapHUD == nullptr)
	{
		StartMapHUD = CreateWidget<UStartMapHUD>(this, StartMapHUDClass);
		if (StartMapHUD)
		{
			StartMapHUD->AddToViewport();
			StartMapHUD->PlayWaterSplashAnimation();
		}
	}
}

void AStartMapPlayerController::InitInputMode()
{
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode); 
	bShowMouseCursor = true; 
}
