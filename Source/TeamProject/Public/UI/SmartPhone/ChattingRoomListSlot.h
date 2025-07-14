// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmartPhoneEnumType.h"
#include "ChatType/ChatType.h"
#include "ChattingRoomListSlot.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UChattingRoomListSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(class USmartPhone * Target);
	void Clear();

	void UpdateChatInfo(const FChatType & ChatType);
	void RemoveChatInfo();
	
public:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void EnterRoom();
	
protected:
	UPROPERTY(EditAnywhere)
	EChattingRoomType ConnectChattingRoomType;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock * Tb_ChatRoomName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock * Tb_RecentMsg;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock * Tb_MsgNum;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage * Img_MsgNumFrame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton * Btn_ChattingRoom;

	UPROPERTY()
	TObjectPtr<class USmartPhone> SmartPhone;

	int UnreadMsgCount = 0;
};
