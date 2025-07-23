// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Component/Interactive/PawnInterActiveComponent.h"

#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Map/Object/Actor/BaseObject.h"
#include "UI/MainHUD/PlayerMainHUD.h"
#include "UI/ObjectInfo/ObjectInfoPanel.h"

void UPawnInterActiveComponent::RegisterSpawnObject(FGameplayTag ObjectTag, ABaseObject* Object,
                                                    bool bRegisterAsInterActedObject)
{
	//checkf(!InteractingObjectsMap.Contains(ObjectTag), TEXT("Object tag not registered"), *ObjectTag.ToString());
	//check(Object);

	if (!Object)
	{
		if (CurrentInteractingObject != Object)
		{
			CurrentInteractingObject = nullptr;
			BroadcastChangeCurrentInteractedObject();
		}

		return;
	}

	if (!InteractingObjectsMap.Contains(ObjectTag))
	{
		InteractingObjectsMap.Emplace(ObjectTag, Object);
	}

	if (CurrentInteractingObject != Object)
	{
		CurrentInteractingObject = Object;
		BroadcastChangeCurrentInteractedObject();
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

void UPawnInterActiveComponent::BroadcastChangeCurrentInteractedObject()
{
	APawn* OwnerPawn = GetOwner<APawn>();

	if (OwnerPawn)
	{
		AMainMapPlayerController* PC = Cast<AMainMapPlayerController>(OwnerPawn->GetController());

		if (PC && PC->IsLocalPlayerController())
		{
			if (ABaseObject::OnCurrentInteractedObjectChanged.IsBound())
				ABaseObject::OnCurrentInteractedObjectChanged.Broadcast(CurrentInteractingObject.Get());

			if (PC->GetPlayerMainHUD()->GetObjectInfoPanel()->OnCurrentInteractedObjectChanged.IsBound())
				PC->GetPlayerMainHUD()->GetObjectInfoPanel()->OnCurrentInteractedObjectChanged.Broadcast(CurrentInteractingObject.Get());
		}
	}
}
