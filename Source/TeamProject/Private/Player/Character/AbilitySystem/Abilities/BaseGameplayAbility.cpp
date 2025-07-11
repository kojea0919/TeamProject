// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Player/Character/Component/Interactive/PawnInterActiveComponent.h"
#include "Player/Character/Component/Repel/RepelComponent.h"

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

// void UBaseGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
// {
// 	Super::OnGiveAbility(ActorInfo, Spec);
//
// 	if (AbilityActivationPolicy == EBaseAbilityActivationPolicy::OnGiven)
// 	{
// 		if (ActorInfo && ActorInfo->AbilitySystemComponent->GetOwnerRole() == ROLE_Authority)
// 		{
// 			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
// 		}
// 	}
// }
//
// void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
// 	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
// 	bool bReplicateEndAbility, bool bWasCancelled)
// {
// 	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
// 	UE_LOG(LogTemp, Warning, TEXT("END (%s) on %s"), *GetName(), GetActorInfo().IsNetAuthority() ? TEXT("Server") : TEXT("Client"));
//
// 	if (AbilityActivationPolicy == EBaseAbilityActivationPolicy::OnGiven)
// 	{
// 		if (ActorInfo && ActorInfo->IsNetAuthority())
// 		{
// 			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
// 		}
// 	}
// }
