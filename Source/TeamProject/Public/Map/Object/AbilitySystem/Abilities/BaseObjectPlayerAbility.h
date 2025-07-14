// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/AbilitySystem/BaseObjectGameplayAbility.h"
#include "BaseObjectPlayerAbility.generated.h"

class AMainMapPlayerController;
class ABaseCharacter;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBaseObjectPlayerAbility : public UBaseObjectGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure, Category="Ability")
	ABaseCharacter* GetCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category="Ability")
	AMainMapPlayerController* GetCharacterControllerFromActorInfo();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ABaseCharacter> CachedCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AMainMapPlayerController> CachedCharacterController;
};
