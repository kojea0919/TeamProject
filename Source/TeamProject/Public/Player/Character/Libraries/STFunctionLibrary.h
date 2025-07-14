// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Player/Character/BaseType/BaseEnumType.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "STFunctionLibrary.generated.h"


class UObjectAbilitySystemComponent;
struct FGameplayTag;
class USTAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API USTFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static USTAbilitySystemComponent* NativeGetAbilitySystemComponentFromActor(AActor* Actor);
	static UObjectAbilitySystemComponent* NativeGetObjectAbilitySystemComponentFromActor(AActor* Actor);
	

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void AddTagToActor(AActor* Actor, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void RemoveTagFromActor(AActor* Actor, FGameplayTag Tag);

	static bool NativeActorHasTag(AActor* Actor, FGameplayTag Tag);
	
};
