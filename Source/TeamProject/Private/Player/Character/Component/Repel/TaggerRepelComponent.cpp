// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Component/Repel/TaggerRepelComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameTag/STGamePlayTags.h"
#include "Map/Object/Actor/BaseWeapon.h"
#include "Player/Character/Libraries/STFunctionLibrary.h"

UTaggerRepelComponent::UTaggerRepelComponent()
{
	SetIsReplicatedByDefault(true);
}

ABaseWeapon* UTaggerRepelComponent::GetTaggerCarriedWeaponByTag(FGameplayTag Tag)
{
	return Cast<ABaseWeapon>(GetCharacterCarriedWeaponByTag(Tag));
}

void UTaggerRepelComponent::OnHitTargetActor(AActor* HitActor, const FHitResult& HitResult)
{
	// UE_LOG(LogTemp, Error, TEXT("OnHitTargetActor Called for %s"), *HitActor->GetName());
	// if (OverlappedActors.Contains(HitActor))
	// {
	// 	return;
	// }
	//
	// OverlappedActors.AddUnique(HitActor);
	//
	// UAbilitySystemComponent* AbilitySystemComponent = USTFunctionLibrary::NativeGetParentAbilitySystemComponentFromActor(HitActor);
	// if (AbilitySystemComponent != nullptr)
	// {
	// 	FGameplayEventData Data;
	// 	Data.EventTag = STGamePlayTags::Event_OnHammerHit;
	// 	Data.Instigator = GetOwningPawn();
	// 	Data.Target = HitActor;
	//
	// 	FGameplayAbilityTargetDataHandle TargetDataHandle;
	// 	TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
	// 	Data.TargetData = TargetDataHandle;
	//
	// 	AbilitySystemComponent->HandleGameplayEvent(STGamePlayTags::Event_OnHammerHit, &Data);
	//
	// 	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitResult.GetActor(), STGamePlayTags::Event_OnHammerHit, Data);
	// }

	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);

	ApplyHammerHit(HitActor, HitResult);
	Multicast_ApplyHammerHit(HitActor, HitResult);
}


void UTaggerRepelComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	return;
}

ABaseWeapon* UTaggerRepelComponent::GetTaggerCurrentEquippedWeapon() const
{
	return Cast<ABaseWeapon>(GetCharacterCurrentEquippedWeapon());
}

void UTaggerRepelComponent::ApplyHammerHit(AActor* HitActor, const FHitResult& HitResult)
{
	UAbilitySystemComponent* AbilitySystemComponent = USTFunctionLibrary::NativeGetParentAbilitySystemComponentFromActor(HitActor);
	if (!AbilitySystemComponent) return;

	FGameplayEventData EventData;
	EventData.EventTag = STGamePlayTags::Event_OnHammerHit;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
	EventData.TargetData = TargetDataHandle;

	AbilitySystemComponent->HandleGameplayEvent(STGamePlayTags::Event_OnHammerHit, &EventData);
}

void UTaggerRepelComponent::Multicast_ApplyHammerHit_Implementation(AActor* HitActor, const FHitResult& HitResult)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	ApplyHammerHit(HitActor, HitResult);
}

void UTaggerRepelComponent::Server_SendHammerHitEvent_Implementation(AActor* HitActor, const FHitResult& HitResult)
{
	UAbilitySystemComponent* AbilitySystemComponent = USTFunctionLibrary::NativeGetParentAbilitySystemComponentFromActor(HitActor);
	if (AbilitySystemComponent != nullptr)
	{
		FGameplayEventData Data;
		Data.EventTag = STGamePlayTags::Event_OnHammerHit;
		Data.Instigator = GetOwningPawn();
		Data.Target = HitActor;

		FGameplayAbilityTargetDataHandle TargetDataHandle;
		TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
		Data.TargetData = TargetDataHandle;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitResult.GetActor(), STGamePlayTags::Event_OnHammerHit, Data);
	}
}

