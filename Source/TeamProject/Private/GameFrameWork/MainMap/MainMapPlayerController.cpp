// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Player/Character/Input/STEnhancedInputComponent.h"
#include "Player/Character/PlayerState/STPlayerState.h"
#include "UI/MainHUD/PlayerMainHUD.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"

void AMainMapPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		InitInputMode();
		InitHUD();
	}
}

void AMainMapPlayerController::UpdateRemainTime(int Second)
{
	if (PlayerMainHUD)
	{
		PlayerMainHUD->UpdateRemainTime(Second);
	}
}

void AMainMapPlayerController::InitInputMode()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AMainMapPlayerController::InitHUD()
{
	if (PlayerMainHUD == nullptr && nullptr != PlayerMainHUDWidgetClass)
	{
		PlayerMainHUD = CreateWidget<UPlayerMainHUD>(this, PlayerMainHUDWidgetClass);
		if (PlayerMainHUD)
		{
			PlayerMainHUD->AddToViewport();			
		}
	}
}


// 추가 부분

void AMainMapPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (USTEnhancedInputComponent* STInputComp = Cast<USTEnhancedInputComponent>(InputComponent))
	{
		STInputComp->BindAbilityAction(STInputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);
	}
}

void AMainMapPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if (IsValid(GetSTAbilitySystemComponent()))
	{
		STAbilitySystemComp->AbilityInputPressed(InputTag);
	}
}

void AMainMapPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if (IsValid(GetSTAbilitySystemComponent()))
	{
		STAbilitySystemComp->AbilityInputReleased(InputTag);
	}
}

USTAbilitySystemComponent* AMainMapPlayerController::GetSTAbilitySystemComponent()
{
	if (!IsValid(STAbilitySystemComp))
	{
		if (const ASTPlayerState* STPlayerState = GetPlayerState<ASTPlayerState>())
		{
			STAbilitySystemComp = STPlayerState->GetSTAbilitySystemComponent();
		}
	}
	return STAbilitySystemComp;
}
