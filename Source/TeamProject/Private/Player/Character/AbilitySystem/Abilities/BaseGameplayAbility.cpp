// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Player/Character/Component/Interactive/PawnInterActiveComponent.h"
#include "Player/Character/Component/Repel/RepelComponent.h"
#include "Player/Character/Component/Repel/TaggerRepelComponent.h"

URepelComponent* UBaseGameplayAbility::GetRepelComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<URepelComponent>();
}

UPawnInterActiveComponent* UBaseGameplayAbility::GetPawnInterActiveComponent() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnInterActiveComponent>();
}

USTAbilitySystemComponent* UBaseGameplayAbility::GetSTAbilitySystemComponentFromActorInfo() const
{
	return Cast<USTAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UBaseGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& SpecHandle)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	check(ASC && SpecHandle.IsValid());

	return GetSTAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, ASC);
	
}

FActiveGameplayEffectHandle UBaseGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& SpecHandle, EBaseSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);
	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EBaseSuccessType::Success : EBaseSuccessType::Failed;
	return ActiveGameplayEffectHandle;
}

