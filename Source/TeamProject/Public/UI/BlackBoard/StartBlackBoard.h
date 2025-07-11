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
	UFUNCTION()
	void ClickStartButton();
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Start;
};
