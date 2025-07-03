// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/Component/Repel/RepelComponent.h"
#include "RunnerRepelComponent.generated.h"


class ARunnerWaterGun;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API URunnerRepelComponent : public URepelComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Repel")
	ARunnerWaterGun* GetRunnerCarriedWaterGunByTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Repel")
	ARunnerWaterGun* GetRunerCurrentEquippedWaterGun() const;
	
};
