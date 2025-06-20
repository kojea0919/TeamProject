// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/AmimInstance/CharacterAnimInstance.h"
#include "RunnerAnimInstance.generated.h"


class ARunnerCharacter;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API URunnerAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reference")
	ARunnerCharacter* OwningRunnerCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Relax")
	bool bShouldEnterRelaxState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Relax")
	float EnterRelaxStateThresold = 5.0f;

	float IdleElapedTime;
	
};
