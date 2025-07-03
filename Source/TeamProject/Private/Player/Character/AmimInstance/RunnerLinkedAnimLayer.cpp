// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AmimInstance/RunnerLinkedAnimLayer.h"

#include "Player/Character/AmimInstance/Runner/RunnerAnimInstance.h"

URunnerAnimInstance* URunnerLinkedAnimLayer::GetRunnerAnimInstance() const
{
	return Cast<URunnerAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
