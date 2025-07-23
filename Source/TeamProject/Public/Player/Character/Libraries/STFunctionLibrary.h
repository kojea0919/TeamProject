// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Player/Character/BaseType/BaseEnumType.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "STFunctionLibrary.generated.h"


class URepelComponent;
class UAbilitySystemComponent;
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
	static UAbilitySystemComponent* NativeGetParentAbilitySystemComponentFromActor(AActor* Actor);
	static USTAbilitySystemComponent* NativeGetAbilitySystemComponentFromActor(AActor* Actor);
	static UObjectAbilitySystemComponent* NativeGetObjectAbilitySystemComponentFromActor(AActor* Actor);
	

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void AddTagToActor(AActor* Actor, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void RemoveTagFromActor(AActor* Actor, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FunctionLibrary")
	static bool NativeActorHasTag(AActor* Actor, FGameplayTag Tag);

	static URepelComponent* NativeGetRepelComponentFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary",
		meta = (DisplayName = "Get RepelComponent From Actor", ExpandEnumAsExecs = "OutValid"))
	static URepelComponent* BP_GetRepelComponentFromActor(AActor* Actor, EBaseValidType& OutValid);
	
};
