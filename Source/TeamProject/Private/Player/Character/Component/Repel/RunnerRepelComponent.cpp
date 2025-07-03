// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Component/Repel/RunnerRepelComponent.h"

#include "Map/Object/Actor/WaterGun/BaseWaterGun.h"

ABaseWaterGun* URunnerRepelComponent::GetRunnerCarriedWaterGunByTag(FGameplayTag Tag)
{
	return Cast<ABaseWaterGun>(GetCharacterCarriedWaterGunByTag(Tag));
}

ABaseWaterGun* URunnerRepelComponent::GetRunerCurrentEquippedWaterGun() const
{
	return Cast<ABaseWaterGun>(GetCharacterCurrentEquippedWaterGun());
}
