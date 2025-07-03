// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Libraries/STFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"

USTAbilitySystemComponent* USTFunctionLibrary::NativeGetAbilitySystemComponentFromActor(AActor* Actor)
{
	check (Actor);

	return CastChecked<USTAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
}

void USTFunctionLibrary::AddTagToActor(AActor* Actor, FGameplayTag Tag)
{
	USTAbilitySystemComponent* ASC = NativeGetAbilitySystemComponentFromActor(Actor);

	if (!ASC->HasMatchingGameplayTag(Tag))
	{
		ASC->AddLooseGameplayTag(Tag);
	}
}

void USTFunctionLibrary::RemoveTagFromActor(AActor* Actor, FGameplayTag Tag)
{
	USTAbilitySystemComponent* ASC = NativeGetAbilitySystemComponentFromActor(Actor);

	if (ASC->HasMatchingGameplayTag(Tag))
	{
		ASC->RemoveLooseGameplayTag(Tag);
	}
}

bool USTFunctionLibrary::NativeActorHasTag(AActor* Actor, FGameplayTag Tag)
{
	USTAbilitySystemComponent* ASC = NativeGetAbilitySystemComponentFromActor(Actor);
	return ASC->HasMatchingGameplayTag(Tag);
}
