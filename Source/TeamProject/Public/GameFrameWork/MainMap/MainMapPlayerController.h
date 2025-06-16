// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMapPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AMainMapPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	void UpdateRemainTime(int Second);

	UFUNCTION(BlueprintCallable)
	void SetActiveMic(bool Active);
	
private:
	void InitInputMode();
	void InitHUD();
	
private:
	//Main HUD
	//----------------------------------
	UPROPERTY(EditAnywhere,Category=HUD, meta = (AllowPrivateAccess=true))
	TSubclassOf<class UPlayerMainHUD> PlayerMainHUDWidgetClass;

	UPROPERTY()
	class UPlayerMainHUD* PlayerMainHUD;
	//----------------------------------
};
