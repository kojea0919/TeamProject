// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelfTalkingBubble.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API USelfTalkingBubble : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetText(FText InputText);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage * Img_TalkingBubble;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock * Tb_InputText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USizeBox * Sb_TalkingBubbleFrame;

	UPROPERTY(EditAnywhere)
	float WidthPaddingSize = 30;
};
