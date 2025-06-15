#include "UI/StartMapUI/StartMapHUD.h"
#include "Components/Button.h"
#include "Network/MainGameInstance.h"

void UStartMapHUD::NativeConstruct()
{
	Super::NativeConstruct();


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
