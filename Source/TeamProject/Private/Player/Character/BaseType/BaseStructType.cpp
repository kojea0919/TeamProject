// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BaseType/BaseStructType.h"
#include "Player/Character/AbilitySystem/Abilities/RunnerGameplayAbility.h"


bool FRunnerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
