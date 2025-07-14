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
	void SetImage(UMaterialInstanceDynamic * Material);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> Img_TalkingBubble;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Tb_InputText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USizeBox> Sb_TalkingBubbleFrame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USizeBox> Sb_Frame;
	
	UPROPERTY(EditAnywhere)
	float WidthPaddingSize = 30;
};
