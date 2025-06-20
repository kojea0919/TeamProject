// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "STAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API USTAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassivesToGrant);
	void InitializeDefaultAbilities(const TSubclassOf<UGameplayEffect>& AttributeEffect);

	void AbilityInputPressed(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);

	
	
};
