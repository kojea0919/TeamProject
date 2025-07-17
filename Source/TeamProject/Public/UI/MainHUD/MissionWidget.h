// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UMissionWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MissionText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalNumber;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AboveNumber;
};
