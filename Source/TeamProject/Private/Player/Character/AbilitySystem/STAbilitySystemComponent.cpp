// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"

#include "AbilitySystemComponent.h"
#include "Player/Character/AbilitySystem/Abilities/RunnerGameplayAbility.h"
#include "GameTag/STGamePlayTags.h"
#include "Map/Object/AbilitySystem/BaseObjectGameplayAbility.h"
#include "Map/Object/AbilitySystem/Abilities/BaseObjectPlayerAbility.h"
#include "Map/Object/Definition/Struct/ObjectStruct.h"
#include "Player/Character/BaseType/BaseStructType.h"

void USTAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant)
{
	for (const TSubclassOf<UGameplayAbility>& Ability : AbilitiesToGrant)
	{

		if (FindAbilitySpecFromClass(Ability) != nullptr)
		{
			continue;
		}
		
		FGameplayAbilitySpec AbilitySpec(Ability, 1.f);
				
		if (const UBaseGameplayAbility* STAbility = Ability->GetDefaultObject<UBaseGameplayAbility>())
		{
			if (STAbility->InputTag.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Add Tag: %s to %s"), *STAbility->InputTag.ToString(), *Ability->GetName());
				AbilitySpec.DynamicAbilityTags.AddTag(STAbility->InputTag);
			}
				
		}
		GiveAbility(AbilitySpec);
		
	}
}

void USTAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& PassivesToGrant)
{
	for (const TSubclassOf<UGameplayAbility>& Ability : PassivesToGrant)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void USTAbilitySystemComponent::InitializeDefaultAbilities(const TSubclassOf<UGameplayEffect>& AttributeEffect)
{
	checkf(AttributeEffect, TEXT("No Valid default attributes for this character %s"), *GetAvatarActor()->GetName())
	const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(AttributeEffect, 1.f, ContextHandle);
	ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void USTAbilitySystemComponent::AddReactAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant)
{
	for (const TSubclassOf<UGameplayAbility>& Ability : AbilitiesToGrant)
	{

		if (FindAbilitySpecFromClass(Ability) != nullptr)
		{
			continue;
		}
		
		FGameplayAbilitySpec AbilitySpec(Ability, 1.f);

		GiveAbility(AbilitySpec);
		
	}
}

void USTAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	ABILITYLIST_SCOPE_LOCK()

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle,
						Spec.ActivationInfo.GetActivationPredictionKey());

			if (!Spec.IsActive())
			{				
				TryActivateAbility(Spec.Handle);
			}
		}		
	}
}

void USTAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid() || !InputTag.MatchesTag(STGamePlayTags::Input_Hold))
	{
		return;
	}

	ABILITYLIST_SCOPE_LOCK()

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle,
				Spec.ActivationInfo.GetActivationPredictionKey());
			
			CancelAbilityHandle(Spec.Handle);
		}
	}
}

void USTAbilitySystemComponent::GrantRunnerWaterGunAbility(const TArray<FPlayerAbilitySet>& WaterGunAbilities,
	int32 Level, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (WaterGunAbilities.IsEmpty())
	{
		return;
	}

	for (const FPlayerAbilitySet& WaterGunAbilitySet : WaterGunAbilities)
	{
		if (!WaterGunAbilitySet.IsValid()) continue;

		FGameplayAbilitySpec Spec(WaterGunAbilitySet.AbilityToGrant);
		Spec.SourceObject = GetAvatarActor();
		Spec.Level = Level;
		Spec.DynamicAbilityTags.AddTag(WaterGunAbilitySet.InputTag);
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(Spec));
		
	}
}

void USTAbilitySystemComponent::RemoveGrantedRunnerWaterGunAbilities(
	TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove)
{
	if (SpecHandlesToRemove.IsEmpty()) return;

	for (FGameplayAbilitySpecHandle& SpecHandle : SpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	SpecHandlesToRemove.Empty();
}
