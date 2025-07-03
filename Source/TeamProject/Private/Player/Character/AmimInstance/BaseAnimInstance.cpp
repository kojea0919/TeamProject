// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AmimInstance/BaseAnimInstance.h"

#include "Player/Character/Libraries/STFunctionLibrary.h"

bool UBaseAnimInstance::OwnerHasTag(FGameplayTag Tag) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return USTFunctionLibrary::NativeActorHasTag(OwningPawn, Tag);
	}

	return false;
}
