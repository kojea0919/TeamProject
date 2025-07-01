// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShowRole.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UShowRole : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();

	void ShowRole(bool IsTagger);
	
protected:
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;
	
	UPROPERTY(BlueprintReadOnly, meta= (BindWidget))
	TObjectPtr<UUserWidget> W_Tagger;

	UPROPERTY(BlueprintReadOnly, meta= (BindWidget))
	TObjectPtr<UUserWidget> W_Runner;
};
