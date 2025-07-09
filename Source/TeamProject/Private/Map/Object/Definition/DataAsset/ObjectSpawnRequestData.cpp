// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Definition/DataAsset/ObjectSpawnRequestData.h"

#include "Map/Object/Actor/BaseObject.h"

bool UObjectSpawnRequestData::AddRequestData(const FSpawnRequest& SpawnRequest)
{
	bool bExists = SpawnRequests.ContainsByPredicate([&SpawnRequest](const FSpawnRequest& Request)
	{
	   return Request.ObjectTypeTag == SpawnRequest.ObjectTypeTag;
	});
    
	if (bExists)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s tag is already registered."), *SpawnRequest.ObjectTypeTag.ToString());
		return false;
	}

	if (SpawnRequest.Quantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid quantity."));
		return false;
	}

	SpawnRequests.Add(SpawnRequest);
	return true;
}

bool UObjectSpawnRequestData::RemoveRequestData(FGameplayTag Tag)
{
	int32 RemovedCount = SpawnRequests.RemoveAll([Tag](const FSpawnRequest& SpawnRequest)
	{
		return SpawnRequest.ObjectTypeTag == Tag;
	});

	if (RemovedCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s tag is not registered"), *Tag.ToString());
		return false;
	}

	return true;
}

void UObjectSpawnRequestData::Clear()
{
	SpawnRequests.Empty();
}
