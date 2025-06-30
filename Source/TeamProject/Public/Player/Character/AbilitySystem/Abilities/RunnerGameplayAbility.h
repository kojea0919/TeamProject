// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "RunnerGameplayAbility.generated.h"

class ARunnerCharacter;
class AMainMapPlayerController;

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

private:
	TWeakObjectPtr<ARunnerCharacter> CachedRunnerCharacter;
	TWeakObjectPtr<AMainMapPlayerController> CachedMainMapPlayerController;
	
};
