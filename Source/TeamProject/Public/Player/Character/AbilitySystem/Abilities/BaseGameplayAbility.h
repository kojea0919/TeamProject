// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"
#include "Player/Character/BaseType/BaseEnumType.h"
#include "BaseGameplayAbility.generated.h"

class UPawnInterActiveComponent;
class URepelComponent;


/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Input")
	FGameplayTag InputTag;

	UFUNCTION(BlueprintPure, Category = "Ability")
	URepelComponent* GetRepelComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UPawnInterActiveComponent* GetPawnInterActiveComponent() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	USTAbilitySystemComponent* GetSTAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& SpecHandle);

	UFUNCTION(BlueprintCallable, Category ="Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle to TargetActor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& SpecHandle, EBaseSuccessType& OutSuccessType);


	
};
