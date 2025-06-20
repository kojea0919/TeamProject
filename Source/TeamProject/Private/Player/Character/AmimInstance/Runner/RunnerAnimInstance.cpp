// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AmimInstance/Runner/RunnerAnimInstance.h"

#include "Player/Character/RunnerCharacter.h"

void URunnerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningRunnerCharacter)
	{
		OwningRunnerCharacter = Cast<ARunnerCharacter>(OwningRunnerCharacter);
	}
}

void URunnerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElapedTime = 0.0f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElapedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElapedTime >= EnterRelaxStateThresold);
	}
	
}
