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

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Call");
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
