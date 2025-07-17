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

USTRUCT(BlueprintType)
struct FAttachRepData
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* TargetActor = nullptr;

	UPROPERTY()
	FName AttachSocket;

	UPROPERTY()
	FName ParentComponentName;

	UPROPERTY()
	bool bAttached = false;
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemTexture;
};