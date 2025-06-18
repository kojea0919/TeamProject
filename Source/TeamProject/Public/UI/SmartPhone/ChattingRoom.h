// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/SmartPhone/SmartPhoneEnumType.h"
#include "ChattingRoom.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UChattingRoom : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(class USmartPhone * Target);

private:
	UPROPERTY()
	TObjectPtr<class USmartPhone> SmartPhone;

	UPROPERTY(EditAnywhere)
	EChattingRoomType RoomType;
};
