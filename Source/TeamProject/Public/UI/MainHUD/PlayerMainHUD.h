// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainHUD.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UPlayerMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetHandSlotText(FText Text);

	UFUNCTION(BlueprintCallable)
	void SetBagSlotText(FText Text);

	UFUNCTION(BlueprintCallable)
	void ChangeItemSlot();

	void UpdateRemainTime(int Second);

	void SetActiveMic(bool Active);
	void PlayTalkingAnimation();

	void Init() const;

	void AddAllChatSelfMessage(const FText& Text);
	void AddAllChatOtherMessage(const FText& Text, const FString& NickName);

	void AddTeamChatSelfMessage(const FText& Text);
	void AddTeamChatOtherMessage(const FText& Text, const FString& NickName);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UPlayerStateText> W_PlayerStateText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USpeedBar> W_SpeedBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UHealthbar> W_HealthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UPlayerItemSlot> W_PlayerHandSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UPlayerItemSlot> W_PlayerBagSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Tb_RemainMinute;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Tb_RemainSecond;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> Img_MicOn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> Img_MicOff;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USmartPhone> W_SmartPhone;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> TalkingAnimation;
	
};
