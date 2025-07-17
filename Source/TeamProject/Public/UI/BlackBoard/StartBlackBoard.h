// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartBlackBoard.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UStartBlackBoard : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void Init();
	
	UFUNCTION()
	void ClickStartButton();

	UFUNCTION()
	void ClickDurationDecreaseButton();
	UFUNCTION()
	void ClickDurationIncreaseButton();

	UFUNCTION()
	void ClickTaggerCntDecreaseButton();
	UFUNCTION()
	void ClickTaggerCntIncreaseButton();

	UFUNCTION()
	void ClickGraffitiCntDecreaseButton();
	UFUNCTION()
	void ClickGraffitiCntIncreaseButton();

	UFUNCTION()
	void ClickGameModeLeftButton();
	UFUNCTION()
	void ClickGameModeRightButton();

	UFUNCTION()
	void ClickTaggerStartTimeDecreaseButton();
	UFUNCTION()
	void ClickTaggerStartTimeIncreaseButton();
	
	void PlayFadeIn();
	void PlayFadeOut();

private:
	void SetDurationText(int Time);
	void SetTaggerCntText(int Cnt);
	void SetGraffitiCntText(int Cnt);
	void SetTaggerStartTimeText(int Time);
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Start;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_DurationDecrease;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_DurationIncrease;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Tb_Duration;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_TaggerCntDecrease;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_TaggerCntIncrease;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Tb_TaggerCnt;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GraffitiCntDecrease;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GraffitiCntIncrease;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Tb_GraffitiCnt;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameModeLeft;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameModeRight;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Tb_GameMode;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_TaggerStartTimeDecrease;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_TaggerStartTimeIncrease;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Tb_TaggerStartTime;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation * FadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation * FadeOut;
};
