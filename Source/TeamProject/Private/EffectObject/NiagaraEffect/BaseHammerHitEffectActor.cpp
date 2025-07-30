// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectObject/NiagaraEffect/BaseHammerHitEffectActor.h"

#include "GameTag/STGamePlayTags.h"
#include "AbilitySystemGlobals.h"

void ABaseHammerHitEffectActor::EffectSetUp(const ABaseCharacter* Player, const ABaseObject* Object)
{
	Super::EffectSetUp(Player, Object);

	FGameplayTag CueTag = STGamePlayTags::Event_OnHammerHit;

	FGameplayEffectContextHandle ContextHandle;
}
