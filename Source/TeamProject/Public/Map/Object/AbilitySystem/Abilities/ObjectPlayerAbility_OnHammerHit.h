// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/AbilitySystem/Abilities/BaseObjectPlayerAbility.h"
#include "ObjectPlayerAbility_OnHammerHit.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UObjectPlayerAbility_OnHammerHit : public UBaseObjectPlayerAbility
{
	GENERATED_BODY()
	UObjectPlayerAbility_OnHammerHit();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> Hammer_Damage;

	UPROPERTY()
	FActiveGameplayEffectHandle Damage_Handle;
};
