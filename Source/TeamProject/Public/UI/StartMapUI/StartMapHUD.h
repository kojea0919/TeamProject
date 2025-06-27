#pragma once

#include "CoreMinimal.h"
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
	
protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> StartWaterSplash;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> StartButtonClick;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_GameStart;
	
private:
	//UPROPERTY()
	//class UButton* CreateSessionButton;

	//UPROPERTY()
	//class UButton* JoinSessionButton;
};
