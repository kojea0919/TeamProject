// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameStart.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UInGameStart : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void StartGame();

	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton * Btn_GameStart;
};
