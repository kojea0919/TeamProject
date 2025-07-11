// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "RunnerGameplayAbility.generated.h"

class URunnerInterActiveComponent;
class ARunnerCharacter;
class AMainMapPlayerController;
class URunnerRepelComponent;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API URunnerGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category ="Ability")
	ARunnerCharacter* GetRunnerCharacterFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category ="Ability")
	AMainMapPlayerController* GetMainMapPlayerControllerFromRunnerInfo();

	UFUNCTION(BlueprintPure, Category ="Ability")
	URunnerRepelComponent* GetRunnerRepelComponentFromRunnerInfo();

	UFUNCTION(BlueprintPure, Category ="Ability")
	URunnerInterActiveComponent* GetRunnerInterActiveComponent();

private:
	TWeakObjectPtr<ARunnerCharacter> CachedRunnerCharacter;
	TWeakObjectPtr<AMainMapPlayerController> CachedMainMapPlayerController;
	
};
