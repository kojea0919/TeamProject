#pragma once

#include "GameplayTagContainer.h"
#include "BaseStructType.generated.h"

class URunnerGameplayAbility;
class URunnerLinkedAnimLayer;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FRunnerAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URunnerGameplayAbility> AbilityToGrant;

	bool IsValid() const;
	
};

USTRUCT(BlueprintType)
struct FWaterGunData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URunnerLinkedAnimLayer> WaterGunAnimLayerLink;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WaterGunInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FRunnerAbilitySet> InputTags;
	
};
