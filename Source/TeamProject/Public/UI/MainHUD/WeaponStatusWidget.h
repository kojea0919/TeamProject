// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponStatusWidget.generated.h"

class ABaseObject;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UWeaponStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateWeaponStatusUI();

	UFUNCTION()
	ABaseObject* GetCurrentEquippedWeapon();

	UFUNCTION()
	bool IsCurrentEquippedWeaponActive();
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponStatus;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_StatusOwner;
};
