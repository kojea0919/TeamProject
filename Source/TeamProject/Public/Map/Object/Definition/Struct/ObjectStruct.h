#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/Engine.h"
#include "ObjectStruct.generated.h"  // 중요: 반드시 마지막에 include

class UInputMappingContext;

USTRUCT(BlueprintType)
struct TEAMPROJECT_API FPlayerAbilitySet  
{
	GENERATED_BODY()

	FPlayerAbilitySet()
	{
		InputTag = FGameplayTag::EmptyTag;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="InputTag"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct TEAMPROJECT_API FObjectDataStruct 
{
	GENERATED_BODY()
    
	FObjectDataStruct()
	{
		ObjectInputMappingContext = nullptr;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* ObjectInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Ability", meta=(TitleProperty="InputTag"))
	TArray<FPlayerAbilitySet> TaggerAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Ability", meta=(TitleProperty="InputTag"))
	TArray<FPlayerAbilitySet> RunnerAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Ability", meta=(TitleProperty="InputTag"))
	TArray<FPlayerAbilitySet> SharedAbilities;
    
	TArray<FPlayerAbilitySet> GetAbility(const AActor* Player) const;
};