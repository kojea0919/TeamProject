// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/AbilitySystem/Abilities/BaseObjectSelfAbility.h"

#include "Map/Object/Actor/BaseObject.h"

ABaseObject* UBaseObjectSelfAbility::GetObjectFromActorInfo()
{
	if (!CachedObject.IsValid())
	{
		CachedObject = Cast<ABaseObject>(CurrentActorInfo->AvatarActor);
	}

	return CachedObject.IsValid() ? CachedObject.Get() : nullptr;
}
