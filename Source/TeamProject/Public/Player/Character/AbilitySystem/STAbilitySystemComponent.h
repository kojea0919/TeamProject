// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "STAbilitySystemComponent.generated.h"

struct FPlayerAbilitySet;
struct FRunnerAbilitySet;
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
	void AddReactAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant);

	void AbilityInputPressed(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);

	// 물총에 포함되어있는 어빌리티를 부여
	UFUNCTION(BlueprintCallable, Category = "Ability", meta = (Level = "1"))
	void GrantRunnerWaterGunAbility(const TArray<FPlayerAbilitySet>& WaterGunAbilities, int32 Level,
		TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	// 물총에 적용된 스펙핸들 제거
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveGrantedRunnerWaterGunAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove);

	
	
};
