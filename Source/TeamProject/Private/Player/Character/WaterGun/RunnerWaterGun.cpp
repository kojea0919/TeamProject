// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/WaterGun/RunnerWaterGun.h"

void ARunnerWaterGun::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles)
{
	GrantedAbilitySpecHandles = SpecHandles;
}

TArray<FGameplayAbilitySpecHandle> ARunnerWaterGun::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
