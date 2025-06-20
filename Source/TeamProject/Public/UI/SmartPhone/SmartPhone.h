// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmartPhoneEnumType.h"
#include "SmartPhone.generated.h"
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API USmartPhone : public UUserWidget
{
	GENERATED_BODY()

// public:
// 	virtual void NativeConstruct() override;
	
public:
	void InitSmartPhone();

	void ChangeScreen(ESmartPhoneScreenState ScreenState);

	//단톡방에 메시지 추가
	//--------------------------------------------------------------------
	void AddAllChatSelfMessage(const FText & Text);
	void AddAllChatOtherMessage(const FText & Text, const FString & NickName);
	//--------------------------------------------------------------------

	//팀방에 메시지 추가
	//--------------------------------------------------------------------
	void AddTeamChatSelfMessage(const FText & Text);
	void AddTeamChatOtherMessage(const FText & Text, const FString & NickName);
	//--------------------------------------------------------------------
private:
	void UpdateScreen() const;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UChattingRoomList> W_ChattingRoomList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UChattingRoom> W_TeamChattingRoom;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UChattingRoom> W_AllChattingRoom;

private:
	ESmartPhoneScreenState CurScreenState;	
};
