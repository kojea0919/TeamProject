// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Map/Object/Definition/Struct/ObjectStruct.h"
#include "BaseObject.generated.h"

struct FGameplayAbilitySpecHandle;
struct FObjectStruct;

UCLASS()
class TEAMPROJECT_API ABaseObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseObject();

	FGameplayTag ObjectTypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ObjectData")
	FObjectDataStruct ObjectData;

	void SetGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles);
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
