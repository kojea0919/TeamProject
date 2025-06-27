// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Definition/DataAsset/ObjectSpawnData.h"
#include "Map/Object/Actor/BaseObject.h"

TArray<TSubclassOf<ABaseObject>> UObjectSpawnData::GetAvailableClasses(FGameplayTag ObjectTypeTag) const
{
	// 해당 태그와 일치하는 매핑 찾기
	for (const FObjectClassMapping& Mapping : ObjectTypeMappings)
	{
		if (Mapping.ObjectTypeTag.MatchesTagExact(ObjectTypeTag))
		{
			return Mapping.AvailableClasses;
		}
	}
    
	// 일치하는 매핑이 없으면 빈 배열 반환
	return TArray<TSubclassOf<ABaseObject>>();
}

TSubclassOf<ABaseObject> UObjectSpawnData::GetRandomClass(FGameplayTag ObjectTypeTag) const
{
	TArray<TSubclassOf<ABaseObject>> AvailableClasses = GetAvailableClasses(ObjectTypeTag);
    
	if (AvailableClasses.IsEmpty())
	{
		return nullptr;
	}
    
	// 랜덤하게 하나 선택
	int32 RandomIndex = FMath::RandRange(0, AvailableClasses.Num() - 1);
	return AvailableClasses[RandomIndex];
}