// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Component/Repel/RepelComponent.h"

#include "Map/Object/Actor/WaterGun/BaseWaterGun.h"

void URepelComponent::RegisterSpawnedWaterGun(FGameplayTag WaterGunTag, ABaseWaterGun* WaterGun,
                                              bool bRegisterAsEquippedWeapon)
{
	checkf(!RunnerCarriedWaterGunMap.Contains(WaterGunTag), TEXT("%s has already been as Carried WaterGun"), *WaterGunTag.ToString());
	check(WaterGun)

	RunnerCarriedWaterGunMap.Emplace(WaterGunTag, WaterGun);

	UE_LOG(LogTemp, Warning, TEXT("WaterGun [%s] registered to map. Actor: %s"),
		   *WaterGunTag.ToString(),
		   *GetNameSafe(WaterGun));

	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWaterGunTag = WaterGunTag;

		UE_LOG(LogTemp, Warning, TEXT("Has been registered using the tag : %s"), *WaterGunTag.ToString());
		
	}
}

ABaseWaterGun* URepelComponent::GetCharacterCarriedWaterGunByTag(FGameplayTag WaterGunTag) const
{
	UE_LOG(LogTemp, Warning, TEXT("WaterGunTag [%s] registered to map"),
		   *WaterGunTag.ToString());

	if (RunnerCarriedWaterGunMap.Contains(WaterGunTag))
	{
		if (ABaseWaterGun* const* FoundWaterGun = RunnerCarriedWaterGunMap.Find(WaterGunTag))
		{
			return *FoundWaterGun;
		}
	}
	return nullptr;
}

ABaseWaterGun* URepelComponent::GetCharacterCurrentEquippedWaterGun() const
{
	if (!CurrentEquippedWaterGunTag.IsValid())
	{
		return nullptr;
	}

	return GetCharacterCarriedWaterGunByTag(CurrentEquippedWaterGunTag);
}
