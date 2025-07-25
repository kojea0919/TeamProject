// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/Component/Repel/RepelComponent.h"
#include "TaggerRepelComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UTaggerRepelComponent : public URepelComponent
{
	GENERATED_BODY()

public:

	UTaggerRepelComponent();
	
	UFUNCTION(BlueprintPure, Category = "Repel")
	ABaseWeapon* GetTaggerCarriedWeaponByTag(FGameplayTag Tag);
	
	UFUNCTION(BlueprintCallable, Category = "Repel")
	ABaseWeapon* GetTaggerCurrentEquippedWeapon() const;

	
	
};
