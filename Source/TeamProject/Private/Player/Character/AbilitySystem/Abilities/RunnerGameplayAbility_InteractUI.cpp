// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AbilitySystem/Abilities/RunnerGameplayAbility_InteractUI.h"

void URunnerGameplayAbility_InteractUI::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp,Warning,TEXT("ActivateAbility"));
}

void URunnerGameplayAbility_InteractUI::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UE_LOG(LogTemp,Warning,TEXT("EndAbility"));
}
