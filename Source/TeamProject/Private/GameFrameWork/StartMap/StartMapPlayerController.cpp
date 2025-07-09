#include "GameFrameWork/StartMap/StartMapPlayerController.h"
#include "UI/StartMapUI/StartMapHUD.h"
#include "UI/StartMapUI/IntroMovie.h"

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

void AStartMapPlayerController::EndMovie()
{
	if (StartMapHUD)
	{
		StartMapHUD->SetVisibility(ESlateVisibility::Visible);
		StartMapHUD->PlayWaterSplashAnimation();
	}

	if (IntroMovieWidget)
	{
		IntroMovieWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AStartMapPlayerController::InitHUD()
{
	if (StartMapHUDClass && StartMapHUD == nullptr)
	{
		StartMapHUD = CreateWidget<UStartMapHUD>(this, StartMapHUDClass);
		if (StartMapHUD)
		{
			StartMapHUD->AddToViewport();
			StartMapHUD->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (IntroMovieClass && IntroMovieWidget == nullptr)
	{
		IntroMovieWidget = CreateWidget<UIntroMovie>(this, IntroMovieClass);
		if (IntroMovieWidget)
		{
			IntroMovieWidget->AddToViewport();
		}
	}
}

void AStartMapPlayerController::InitInputMode()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode); 
	bShowMouseCursor = true; 
}
