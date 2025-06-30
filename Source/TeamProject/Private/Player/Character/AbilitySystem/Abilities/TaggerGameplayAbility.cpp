// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AbilitySystem/Abilities/TaggerGameplayAbility.h"

#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Player/Character/TaggerCharacter.h"

ATaggerCharacter* UTaggerGameplayAbility::GetTaggerCharacterFromActorInfo()
{
	if (CachedTaggerCharacter.IsValid())
	{
		CachedTaggerCharacter = Cast<ATaggerCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedTaggerCharacter.IsValid() ? CachedTaggerCharacter.Get() : nullptr;
}

AMainMapPlayerController* UTaggerGameplayAbility::GetMainMapPlayerControllerFromTaggerInfo()
{
	if (CachedMainMapPlayerController.IsValid())
	{
		CachedMainMapPlayerController = Cast<AMainMapPlayerController>(CurrentActorInfo->PlayerController);
	}

	return CachedMainMapPlayerController.IsValid() ? CachedMainMapPlayerController.Get() : nullptr;
}
