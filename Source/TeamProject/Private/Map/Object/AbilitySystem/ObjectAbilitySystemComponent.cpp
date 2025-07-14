// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/AbilitySystem/ObjectAbilitySystemComponent.h"

#include "Map/Object/AbilitySystem/BaseObjectGameplayAbility.h"

void UObjectAbilitySystemComponent::AddAbilityToObject(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant)
{
	for (const TSubclassOf<UGameplayAbility>& Ability : AbilitiesToGrant)
	{
		if (FindAbilitySpecFromClass(Ability) != nullptr)
		{
			continue;
		}
		
		FGameplayAbilitySpec AbilitySpec(Ability, 1.f);
				
		if (const UBaseObjectGameplayAbility* ObjectAbility = Ability->GetDefaultObject<UBaseObjectGameplayAbility>())
		{
			if (ObjectAbility->EventTag.IsValid())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Add Tag: %s to %s"), *STAbility->InputTag.ToString(), *Ability->GetName());
				AbilitySpec.DynamicAbilityTags.AddTag(ObjectAbility->EventTag);
			}
				
		}
		GiveAbility(AbilitySpec);
	}
}