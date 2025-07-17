// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/Component/Interactive/PawnInterActiveComponent.h"
#include "RunnerInterActiveComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API URunnerInterActiveComponent : public UPawnInterActiveComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintPure, Category = "IterActive")
	ABaseObject* GetRunnerCarriedObjectByTag(FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = "IterActive")
	ABaseObject* GetRunnerCurrentInterActedObject() const;

	UFUNCTION(BlueprintPure, Category = "IterActive")
	bool RunnerHasObject(FGameplayTag Tag);
	
};
