#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackProxy.h"
#include "Blueprint/UserWidget.h"
#include "StartMapHUD.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UStartMapHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void PlayWaterSplashAnimation();
	
public:
	UFUNCTION()
	void ClickCreateSessionButton();

	UFUNCTION()
	void ClickJoinSessionButton();

	UFUNCTION()
	void ClickGameStartButton();
	
	void AddToSessionList(FBlueprintSessionResult SessionResult);

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> StartWaterSplash;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> StartButtonClick;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_GameStart;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USessionListBase> WB_SessionListFrame;

private:
	//UPROPERTY()
	//class UButton* CreateSessionButton;

	//UPROPERTY()
	//class UButton* JoinSessionButton;
};
