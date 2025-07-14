// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseObjectGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBaseObjectGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Event")
	FGameplayTag EventTag;
};
