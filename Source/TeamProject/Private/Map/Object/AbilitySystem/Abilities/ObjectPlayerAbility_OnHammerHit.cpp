// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/AbilitySystem/Abilities/ObjectPlayerAbility_OnHammerHit.h"

#include "Player/Character/RunnerCharacter.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"

UObjectPlayerAbility_OnHammerHit::UObjectPlayerAbility_OnHammerHit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// AbilityTrigger 추가
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag("Event.OnHammerHit");
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    
	AbilityTriggers.Add(TriggerData);
}

void UObjectPlayerAbility_OnHammerHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hello"));

	if (!HasAuthority(&ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ARunnerCharacter* RunnerCharacter = Cast<ARunnerCharacter>(GetCharacterFromActorInfo());
	
	if (!RunnerCharacter)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	USTAbilitySystemComponent* RunnerASC = Cast<USTAbilitySystemComponent>(RunnerCharacter->GetAbilitySystemComponent());

	if (!RunnerASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(Hammer_Damage, 1.0f);

	Damage_Handle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
