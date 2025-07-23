// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Component/Repel/RunnerRepelComponent.h"

#include "Map/Object/Actor/BaseWeapon.h"

ABaseWeapon* URunnerRepelComponent::GetRunnerCarriedWaterGunByTag(FGameplayTag Tag) const
{
	return Cast<ABaseWeapon>(GetCharacterCarriedWeaponByTag(Tag));
}

ABaseWeapon* URunnerRepelComponent::GetRunerCurrentEquippedWaterGun() const
{
	return Cast<ABaseWeapon>(GetCharacterCurrentEquippedWeapon());
}
