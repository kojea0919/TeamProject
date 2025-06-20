// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OtherTalkingBubble.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UOtherTalkingBubble : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetInputText(const FText & InputText);
	void SetNickName(const FString & NickName);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage * Img_TalkingBubble;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock * Tb_InputText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USizeBox * Sb_TalkingBubbleFrame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock * Tb_NickName;
	
	UPROPERTY(EditAnywhere)
	float WidthPaddingSize = 30;
};
