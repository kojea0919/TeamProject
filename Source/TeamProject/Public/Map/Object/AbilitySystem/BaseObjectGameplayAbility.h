// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "BaseObjectGameplayAbility.generated.h"

class AMainMapPlayerController;
class ABaseCharacter;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBaseObjectGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category="Ability")
	ABaseCharacter* GetCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category="Ability")
	AMainMapPlayerController* GetCharacterControllerFromActorInfo();

	
private:
	TWeakObjectPtr<ABaseCharacter> CachedCharacter;
	TWeakObjectPtr<AMainMapPlayerController> CachedCharacterController;
};
