// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/AbilitySystem/Abilities/RunnerGameplayAbility.h"
#include "RunnerGameplayAbility_Run.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API URunnerGameplayAbility_Run : public URunnerGameplayAbility
{
	GENERATED_BODY()

public:
	URunnerGameplayAbility_Run();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	void ApplyRunMovementSpeed();
	void ResetMovementSpeed();

protected:

	UPROPERTY()
	float CachedOriginalSpeed = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> StaminaDrain;

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> StaminaRegen;

	FActiveGameplayEffectHandle DrainHandle;
	FActiveGameplayEffectHandle RegenHandle;

	FDelegateHandle StaminaChangedDelegateHandle;

	
	
};
