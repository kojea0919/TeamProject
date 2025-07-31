// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AbilitySystem/Abilities/TaggerGameplayAbility_Run.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Character/TaggerCharacter.h"
#include "Player/Character/AbilitySystem/Attributes/STAttributeSet.h"


UTaggerGameplayAbility_Run::UTaggerGameplayAbility_Run()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UTaggerGameplayAbility_Run::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
				.AddUObject(this, &UTaggerGameplayAbility_Run::OnStaminaChanged);
			}
		}
	}
	ApplyRunMovementSpeed();
}

void UTaggerGameplayAbility_Run::EndAbility(const FGameplayAbilitySpecHandle Handle,
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
			FGameplayEffectSpecHandle RegenSpec = MakeOutgoingGameplayEffectSpec(StaminaRegen, 1.f);
			RegenHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, RegenSpec);
		}

		if (StaminaChangedDelegateHandle.IsValid())
		{
			if (const USTAttributeSet* AttributeSet = ASC->GetSet<USTAttributeSet>())
			{
				ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute())
					.Remove(StaminaChangedDelegateHandle);
			}
			StaminaChangedDelegateHandle.Reset();
		}
		
	}

	ResetMovementSpeed();
}

void UTaggerGameplayAbility_Run::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.f)
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

void UTaggerGameplayAbility_Run::ApplyRunMovementSpeed()
{
	if (ATaggerCharacter* Character = Cast<ATaggerCharacter>(GetAvatarActorFromActorInfo()))
	{
		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
		CachedOriginalSpeed = MovementComponent->MaxWalkSpeed;
		MovementComponent->MaxWalkSpeed = 750.f;
	}
}

void UTaggerGameplayAbility_Run::ResetMovementSpeed()
{
	if (ATaggerCharacter* Character = Cast<ATaggerCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = CachedOriginalSpeed;
	}
}
