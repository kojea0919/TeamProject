// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatType/ChatType.h"
#include "ChattingRoomList.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UChattingRoomList : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(class USmartPhone * Target);
	void Clear();

	void UpdateAllChatInfo(const FChatType & Text);
	void UpdateTeamChatInfo(const FChatType & Text);
	
protected:
	virtual void NativeConstruct() override;;

private:
	UFUNCTION()
	void ChangeMicVolume(float Value);
	
protected:
	UPROPERTY()
	TObjectPtr<class USmartPhone> SmartPhone;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UChattingRoomListSlot> W_AllChatRoomListSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UChattingRoomListSlot> W_TeamChatRoomListSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USlider> S_MicVolumeSlide;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> Img_MicOn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> Img_MicOff;
};
