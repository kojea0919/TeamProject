#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackProxy.h"
#include "Blueprint/UserWidget.h"
#include "StartMapHUD.generated.h"



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
	void ClickGameStartButton();
	
	void AddToSessionList(FBlueprintSessionResult SessionResult);
	void ClearSessionList();
	void SetVisibleSessionLoadImage(bool bVisible);

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> StartWaterSplash;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> StartButtonClick;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ActiveSessionList;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> DeactiveSessionList;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ActiveGameStart;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> DeactiveGameStart;
	
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
