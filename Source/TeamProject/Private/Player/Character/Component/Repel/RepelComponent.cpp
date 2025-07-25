// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Component/Repel/RepelComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/BoxComponent.h"
#include "GameTag/STGamePlayTags.h"
#include "Map/Object/Actor/BaseWeapon.h"
#include "Map/Object/Actor/Hammer/BaseHammer.h"
#include "Player/Character/Component/Repel/TaggerRepelComponent.h"

void URepelComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTag, ABaseObject* Weapon, bool bRegisterAsEquippedWeapon)
{
	
	checkf(!CarriedWeaponMap.Contains(WeaponTag), TEXT("%s has already been as Carried WaterGun"), *WeaponTag.ToString());
		
	CarriedWeaponMap.Emplace(WeaponTag, Weapon);

	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = WeaponTag;
		
	}
}

ABaseObject* URepelComponent::GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
	if (CarriedWeaponMap.Contains(WeaponTag))
	{
		if (ABaseObject* const* FoundWeapon = CarriedWeaponMap.Find(WeaponTag))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

ABaseObject* URepelComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}
