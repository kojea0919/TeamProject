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
	
private:
	void UpdateScreen() const;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UChattingRoomList * W_ChattingRoomList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UChattingRoom * W_TeamChattingRoom;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UChattingRoom * W_AllChattingRoom;

private:
	ESmartPhoneScreenState CurScreenState;	
};
