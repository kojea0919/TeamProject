// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Component/Repel/RepelComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/BoxComponent.h"
#include "GameTag/STGamePlayTags.h"
#include "Map/Object/Actor/BaseWeapon.h"
#include "Map/Object/Actor/Hammer/BaseHammer.h"
#include "Player/Character/Component/Repel/TaggerRepelComponent.h"

void URepelComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTag, ABaseWeapon* Weapon, bool bRegisterAsEquippedWeapon)
{
	
	checkf(!CarriedWeaponMap.Contains(WeaponTag), TEXT("%s has already been as Carried WaterGun"), *WeaponTag.ToString());
		
	CarriedWeaponMap.Emplace(WeaponTag, Weapon);
	
	Weapon->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	Weapon->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	CachedWeapon = Weapon;

	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = WeaponTag;
		
	}
}

ABaseWeapon* URepelComponent::GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const
{
	if (CarriedWeaponMap.Contains(WeaponTag))
	{
		if (ABaseWeapon* const* FoundWeapon = CarriedWeaponMap.Find(WeaponTag))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

ABaseWeapon* URepelComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void URepelComponent::ToggleWeaponCollision(bool bUse, EToggleDamageType ToggleDamageType)
{
	
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		ABaseWeapon* Weapon = CachedWeapon;
		const ABaseHammer* Hammer = Cast<ABaseHammer>(Weapon);

		if (!Hammer)
		{
			return; 
		}

		if (bUse)
		{
			Hammer->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			Hammer->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			OverlappedActors.Empty();
		}
	}
}

void URepelComponent::OnHitTargetActor(AActor* HitActor, const FHitResult& HitResult)
{
	
}

void URepelComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	// 자식에서 구현
}
