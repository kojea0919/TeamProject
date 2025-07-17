// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Definition/Struct/ObjectStruct.h"
#include "Player/Character/AbilitySystem/Abilities/BaseGameplayAbility.h"


FPlayerAbilitySet::FPlayerAbilitySet()
{
	InputTag = FGameplayTag::EmptyTag;
	AbilityToGrant = nullptr;
}

bool FPlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}

TArray<FPlayerAbilitySet> FObjectDataStruct::GetAbility(const AActor* Player) const
{
	TArray<FPlayerAbilitySet> Abilities;
	Abilities.Append(SharedAbilities);
	
	if (Player && Player->ActorHasTag("Player.Tagger"))
		Abilities.Append(TaggerAbilities);
	if (Player && Player->ActorHasTag("Player.Runner"))
		Abilities.Append(RunnerAbilities);

	return Abilities;
}
