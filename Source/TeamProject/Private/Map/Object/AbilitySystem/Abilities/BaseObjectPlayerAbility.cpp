// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/AbilitySystem/Abilities/BaseObjectPlayerAbility.h"

#include "EffectObject/NiagaraEffect/BaseWaterGunBeamEffectActor.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Player/Character/BaseCharacter.h"


ABaseCharacter* UBaseObjectPlayerAbility::GetCharacterFromActorInfo()
{
	if (!CachedCharacter.IsValid())
	{
		CachedCharacter = Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedCharacter.IsValid() ? CachedCharacter.Get() : nullptr;
}


AMainMapPlayerController* UBaseObjectPlayerAbility::GetCharacterControllerFromActorInfo()
{
	if (!CachedCharacterController.IsValid())
	{
		CachedCharacterController = Cast<AMainMapPlayerController>(CurrentActorInfo->PlayerController);
	}

	return CachedCharacterController.IsValid() ? CachedCharacterController.Get() : nullptr;
}

