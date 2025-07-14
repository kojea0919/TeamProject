#include "UI/StartMapUI/StartMapHUD.h"
#include "Components/Button.h"
#include "UI/StartMapUI/SessionListBase.h"

void UStartMapHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_GameStart)
		Btn_GameStart->OnClicked.AddDynamic(this, &UStartMapHUD::ClickGameStartButton);
	
	if (WB_SessionListFrame)
	{
		WB_SessionListFrame->OnFrameExitButtonClicked.BindLambda([this]()
		{
			if (DeactiveSessionList)
				PlayAnimation(DeactiveSessionList);

			if (ActiveGameStart)
				PlayAnimation(ActiveGameStart);
		});
	}
}

void UStartMapHUD::PlayWaterSplashAnimation()
{
	if (nullptr != StartWaterSplash)
	{
		PlayAnimation(StartWaterSplash);
	}
}

void UStartMapHUD::ClickGameStartButton()
{
	if (nullptr != DeactiveGameStart)
		PlayAnimation(DeactiveGameStart);

	if (nullptr != ActiveSessionList)
		PlayAnimation(ActiveSessionList);
	
}

void UStartMapHUD::AddToSessionList(FBlueprintSessionResult SessionResult)
{
	if (WB_SessionListFrame)
	{
		WB_SessionListFrame->AddSessionList(SessionResult);
	}
}

void UStartMapHUD::ClearSessionList()
{
	if (WB_SessionListFrame)
		WB_SessionListFrame->ClearSessionList();
}

void UStartMapHUD::SetVisibleSessionLoadImage(bool bVisible)
{
	if (WB_SessionListFrame)
		WB_SessionListFrame->SetVisibleSessionLoadImage(bVisible);
}
