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

	UE_LOG(LogTemp, Warning, TEXT("RegisterSpawnedWeapon this is: %s"), *GetClass()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("RegisterSpawnedWeapon: %s to %s"), *Weapon->GetName(), *GetOwner()->GetName());
	
	CarriedWeaponMap.Emplace(WeaponTag, Weapon);

	UE_LOG(LogTemp, Warning, TEXT("TaggerRepelComponent Cast 성공: %s"), *GetName());
	Weapon->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	Weapon->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	CachedWeapon = Weapon;

	UE_LOG(LogTemp, Error, TEXT("[RepelComponent] %s :: CachedWeapon = %s"), *GetNameSafe(this), *GetNameSafe(CachedWeapon));
	CurrentEquippedWeaponTag = WeaponTag;
		
	UE_LOG(LogTemp, Warning, TEXT("IsBound? %s"), Weapon->OnWeaponHitTarget.IsBound() ? TEXT("YES") : TEXT("NO"));
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = WeaponTag;

		UE_LOG(LogTemp, Warning, TEXT("Has been registered using the tag : %s"), *WeaponTag.ToString());
		
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
	UE_LOG(LogTemp, Error, TEXT("[RepelComponent] %s :: CachedWeapon = %s"), *GetNameSafe(this), *GetNameSafe(CachedWeapon));
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		ABaseWeapon* Weapon = CachedWeapon;
		const ABaseHammer* Hammer = Cast<ABaseHammer>(Weapon);

		if (!Hammer)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Weapon is null in ToggleWeaponCollision! Did you forget to call RegisterSpawnedWeapon?"));
			return; // 또는 ensureMsgf(false, TEXT("Weapon is null"));
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
	// // 자식에서 구현
	// UE_LOG(LogTemp, Error, TEXT("OnHitTargetActor Called for %s"), *HitActor->GetName());
	// if (OverlappedActors.Contains(HitActor))
	// {
	// 	return;
	// }
	//
	// OverlappedActors.AddUnique(HitActor);
	//
	// FGameplayEventData Data;
	// Data.EventTag = STGamePlayTags::Event_OnHammerHit;
	// Data.Instigator = GetOwningPawn();
	// Data.Target = HitActor;
	//
	// FGameplayAbilityTargetDataHandle TargetDataHandle;
	// TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));  
	// Data.TargetData = TargetDataHandle;
	//
	// UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), STGamePlayTags::Event_OnHammerHit, Data);
}

void URepelComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	// 자식에서 구현
}
