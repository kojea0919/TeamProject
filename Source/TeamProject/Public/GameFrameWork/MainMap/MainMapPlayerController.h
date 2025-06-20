// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"
#include "MainMapPlayerController.generated.h"

class USTInputConfig;

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
	
	UFUNCTION(BlueprintCallable)
	void SetTalkingMic();

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


	// 추가 부분
public:
	
	virtual void SetupInputComponent() override;

protected:

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);

private:
	UPROPERTY()
	TObjectPtr<USTAbilitySystemComponent> STAbilitySystemComp;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Input")
	TObjectPtr<USTInputConfig> STInputConfig;

	USTAbilitySystemComponent* GetSTAbilitySystemComponent();
	
};
