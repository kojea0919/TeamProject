// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThredSafe))
	bool OwnerHasTag(FGameplayTag Tag) const;
	
};
