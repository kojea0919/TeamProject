// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Component/Repel/TaggerRepelComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameTag/STGamePlayTags.h"
#include "Map/Object/Actor/BaseWeapon.h"
#include "Player/Character/Libraries/STFunctionLibrary.h"

UTaggerRepelComponent::UTaggerRepelComponent()
{
	SetIsReplicatedByDefault(true);
}

ABaseWeapon* UTaggerRepelComponent::GetTaggerCarriedWeaponByTag(FGameplayTag Tag)
{
	return Cast<ABaseWeapon>(GetCharacterCarriedWeaponByTag(Tag));
}

ABaseWeapon* UTaggerRepelComponent::GetTaggerCurrentEquippedWeapon() const
{
	return Cast<ABaseWeapon>(GetCharacterCurrentEquippedWeapon());
}

