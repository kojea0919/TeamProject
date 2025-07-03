// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/AbilitySystem/BaseObjectGameplayAbility.h"

#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Player/Character/BaseCharacter.h"

ABaseCharacter* UBaseObjectGameplayAbility::GetCharacterFromActorInfo()
{
	if (!CachedCharacter.IsValid())
	{
		CachedCharacter = Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedCharacter.IsValid() ? CachedCharacter.Get() : nullptr;
}

AMainMapPlayerController* UBaseObjectGameplayAbility::GetCharacterControllerFromActorInfo()
{
	if (!CachedCharacterController.IsValid())
	{
		CachedCharacterController = Cast<AMainMapPlayerController>(CurrentActorInfo->PlayerController);
	}

	return CachedCharacterController.IsValid() ? CachedCharacterController.Get() : nullptr;
}
