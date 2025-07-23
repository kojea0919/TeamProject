// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "TaggerGameplayAbility.generated.h"

class UTaggerRepelComponent;
class ATaggerCharacter;
class AMainMapPlayerController;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UTaggerGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(Blueprintpure, Category = "Ability")
	ATaggerCharacter* GetTaggerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	AMainMapPlayerController* GetMainMapPlayerControllerFromTaggerInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	UTaggerRepelComponent* GetTaggerRepelComponentFromActorInfo();
	

private:
	TWeakObjectPtr<ATaggerCharacter> CachedTaggerCharacter;
	TWeakObjectPtr<AMainMapPlayerController> CachedMainMapPlayerController;
	
};
