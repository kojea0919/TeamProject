// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AbilitySystem/Abilities/RunnerGameplayAbility_Run.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Character/RunnerCharacter.h"
#include "Player/Character/AbilitySystem/Attributes/STAttributeSet.h"

URunnerGameplayAbility_Run::URunnerGameplayAbility_Run()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void URunnerGameplayAbility_Run::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (RegenHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(RegenHandle);
			RegenHandle.Invalidate();
		}

		if (StaminaDrain)
		{
			FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(StaminaDrain, 1.f);
			DrainHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);
		}

		if (!StaminaChangedDelegateHandle.IsValid())
		{
			if (const USTAttributeSet* AttributeSet = ASC->GetSet<USTAttributeSet>())
			{
				StaminaChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute())
					.AddUObject(this, &URunnerGameplayAbility_Run::OnStaminaChanged);
			}
		}
	}
	
	ApplyRunMovementSpeed();
}

void URunnerGameplayAbility_Run::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (DrainHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(DrainHandle);
			DrainHandle.Invalidate();
		}

		if (StaminaRegen)
		{
			FScopedPredictionWindow PredictionScope(ASC, GetCurrentActivationInfo().GetActivationPredictionKey());
			FGameplayEffectSpecHandle RegenSpec = MakeOutgoingGameplayEffectSpec(StaminaRegen, 1.f);
			RegenHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, RegenSpec);
		}
		
	}

	ResetMovementSpeed();
}

void URunnerGameplayAbility_Run::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.1f)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (const USTAttributeSet* AttributeSet = ASC->GetSet<USTAttributeSet>())
		{
			const float MaxStamina = AttributeSet->GetMaxStamina();
			if (Data.NewValue >= MaxStamina && StaminaChangedDelegateHandle.IsValid())
			{
				ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).Remove(StaminaChangedDelegateHandle);
				StaminaChangedDelegateHandle.Reset();
			}
		}
	}
}

void URunnerGameplayAbility_Run::ApplyRunMovementSpeed()
{
	if (ARunnerCharacter* Character = Cast<ARunnerCharacter>(GetAvatarActorFromActorInfo()))
	{
		UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
		CachedOriginalSpeed = Movement->MaxWalkSpeed;
		Movement->MaxWalkSpeed = 600.f;
	}
}

void URunnerGameplayAbility_Run::ResetMovementSpeed()
{
	if (ARunnerCharacter* Character = Cast<ARunnerCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = CachedOriginalSpeed;
	}
}
