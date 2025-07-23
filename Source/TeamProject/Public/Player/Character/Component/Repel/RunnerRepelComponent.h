// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/Component/Repel/RepelComponent.h"
#include "RunnerRepelComponent.generated.h"


class ABaseWeapon;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API URunnerRepelComponent : public URepelComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Repel")
	ABaseWeapon* GetRunnerCarriedWaterGunByTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Repel")
	ABaseWeapon* GetRunerCurrentEquippedWaterGun() const;
	
};
