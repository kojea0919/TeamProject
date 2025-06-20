// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Input/STInputConfig.h"

UInputAction* USTInputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FSTInputAction& Action : NativeInputActions)
	{
		if (Action.InputTag.MatchesTagExact(InInputTag))
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}
