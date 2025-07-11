// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Component/Interactive/PawnInterActiveComponent.h"

void UPawnInterActiveComponent::RegisterSpawnObject(FGameplayTag ObjectTag, ABaseObject* Object,
	bool bRegisterAsInterActedObject)
{
	//checkf(!InteractingObjectsMap.Contains(ObjectTag), TEXT("Object tag not registered"), *ObjectTag.ToString());
	//check(Object);

	if (!Object)
	{
		return;
	}

	if (!InteractingObjectsMap.Contains(ObjectTag))
	{
		InteractingObjectsMap.Emplace(ObjectTag, Object);
	}
	
	if (bRegisterAsInterActedObject)
	{
		CharacterCurrentInterActedObjectTag = ObjectTag;
	}
}

ABaseObject* UPawnInterActiveComponent::GetSpawnObjectByTag(FGameplayTag ObjectTag) const
{
	if (InteractingObjectsMap.Contains(ObjectTag))
	{
		if (ABaseObject* const* FoundObject = InteractingObjectsMap.Find(ObjectTag))
		{
			return *FoundObject;
		}
	}
	return nullptr;
}

ABaseObject* UPawnInterActiveComponent::GetCharacterCurrentInterActedObject() const
{
	if (!CharacterCurrentInterActedObjectTag.IsValid())
	{
		return nullptr;
	}
	return GetSpawnObjectByTag(CharacterCurrentInterActedObjectTag);
}
