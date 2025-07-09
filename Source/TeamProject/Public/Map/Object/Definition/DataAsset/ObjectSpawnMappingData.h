// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ObjectSpawnMappingData.generated.h"

/**
 * 
 */

class ABaseObject;

USTRUCT(BlueprintType)
struct FObjectClassMapping
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="Object"))
	FGameplayTag ObjectTypeTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ABaseObject>> AvailableClasses;
};

UCLASS()
class TEAMPROJECT_API UObjectSpawnMappingData : public UDataAsset
{
	GENERATED_BODY()



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Requests")
	TArray<FObjectClassMapping> ObjectTypeMappings;

	UFUNCTION(BlueprintCallable, Category = "Spawn Mapping Data")
	TArray<TSubclassOf<ABaseObject>> GetAvailableClasses(FGameplayTag ObjectTypeTag) const;

	UFUNCTION(BlueprintCallable, Category = "Spawn Mapping Data")
	TSubclassOf<ABaseObject> GetRandomClass(FGameplayTag ObjectTypeTag) const;
};
