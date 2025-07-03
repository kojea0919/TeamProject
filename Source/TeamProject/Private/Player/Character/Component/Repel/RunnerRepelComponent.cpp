// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Component/Repel/RunnerRepelComponent.h"

#include "Player/Character/WaterGun/RunnerWaterGun.h"

ARunnerWaterGun* URunnerRepelComponent::GetRunnerCarriedWaterGunByTag(FGameplayTag Tag)
{
	return Cast<ARunnerWaterGun>(GetCharacterCarriedWaterGunByTag(Tag));
}

ARunnerWaterGun* URunnerRepelComponent::GetRunerCurrentEquippedWaterGun() const
{
	return Cast<ARunnerWaterGun>(GetCharacterCurrentEquippedWaterGun());
}
