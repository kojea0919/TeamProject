// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/AmimInstance/BaseAnimInstance.h"
#include "RunnerLinkedAnimLayer.generated.h"

class URunnerAnimInstance;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API URunnerLinkedAnimLayer : public UBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	URunnerAnimInstance* GetRunnerAnimInstance() const;
};
