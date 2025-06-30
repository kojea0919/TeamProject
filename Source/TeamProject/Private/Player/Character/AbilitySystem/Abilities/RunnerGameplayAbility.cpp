// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AbilitySystem/Abilities/RunnerGameplayAbility.h"

#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Player/Character/RunnerCharacter.h"

ARunnerCharacter* URunnerGameplayAbility::GetRunnerCharacterFromActorInfo()
{
	if (!CachedRunnerCharacter.IsValid())
	{
		CachedRunnerCharacter = Cast<ARunnerCharacter>(CurrentActorInfo->AvatarActor);
		
	}

	return CachedRunnerCharacter.IsValid() ? CachedRunnerCharacter.Get() : nullptr;
}

AMainMapPlayerController* URunnerGameplayAbility::GetMainMapPlayerControllerFromRunnerInfo()
{
	if (!CachedMainMapPlayerController.IsValid())
	{
		CachedMainMapPlayerController = Cast<AMainMapPlayerController>(CurrentActorInfo->PlayerController);
	}

	return CachedMainMapPlayerController.IsValid() ? CachedMainMapPlayerController.Get() : nullptr;
}
