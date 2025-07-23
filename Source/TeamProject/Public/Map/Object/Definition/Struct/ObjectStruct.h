#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/Engine.h"
#include "ObjectStruct.generated.h"  // 중요: 반드시 마지막에 include

class UGameplayAbility;
class UBaseObjectSelfAbility;
class UBaseObjectGameplayAbility;
class URunnerLinkedAnimLayer;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct TEAMPROJECT_API FPlayerAbilitySet  
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="InputTag"))
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="InputTag"))
	TSubclassOf<UGameplayAbility> AbilityToGrant;

	FPlayerAbilitySet();
	bool IsValid() const;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="React Ability")
	TArray<TSubclassOf<UGameplayAbility>> SelfAbilities;
    
	TArray<FPlayerAbilitySet> GetAbility(const AActor* Player) const;

	// 위젯 업데이트를 위한 추가 데이터
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon;
};