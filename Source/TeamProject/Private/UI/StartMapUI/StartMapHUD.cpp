#include "UI/StartMapUI/StartMapHUD.h"
#include "Components/Button.h"
#include "Network/MainGameInstance.h"
#include "UI/StartMapUI/SessionListBase.h"

void UStartMapHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_GameStart)
		Btn_GameStart->OnClicked.AddDynamic(this, &UStartMapHUD::ClickGameStartButton);
	
	/*CreateSessionButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_CreateSession")));
	if (CreateSessionButton)
	{
		CreateSessionButton->OnClicked.AddDynamic(this, &UStartMapHUD::ClickCreateSessionButton);
	}

	JoinSessionButton = Cast<UButton>(GetWidgetFromName(TEXT("Btn_JoinSession")));
	if (JoinSessionButton)
	{
		JoinSessionButton->OnClicked.AddDynamic(this, &UStartMapHUD::ClickJoinSessionButton);
	}*/
}

void UStartMapHUD::PlayWaterSplashAnimation()
{
	if (nullptr != StartWaterSplash)
	{
		PlayAnimation(StartWaterSplash);
	}
}

void UStartMapHUD::ClickCreateSessionButton()
{
	UMainGameInstance * MainGameInstance = GetGameInstance<UMainGameInstance>();
	if (MainGameInstance)
	{
		MainGameInstance->CreateGameSession();
	}
}

void UStartMapHUD::ClickJoinSessionButton()
{
	UMainGameInstance* MainGameInstance = GetGameInstance<UMainGameInstance>();
	if (MainGameInstance)
	{
		MainGameInstance->JoinGameSession();
	}
}

void UStartMapHUD::ClickGameStartButton()
{
	if (nullptr != StartButtonClick)
		PlayAnimation(StartButtonClick);

	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Call"));
}

void UStartMapHUD::AddToSessionList(FBlueprintSessionResult SessionResult)
{
	if (WB_SessionListFrame)
	{
		WB_SessionListFrame->AddSessionList(SessionResult);
	}
}
