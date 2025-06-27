// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Definition/Struct/ObjectStruct.h"

TArray<FPlayerAbilitySet> FObjectDataStruct::GetAbility(const AActor* Player) const
{
	TArray<FPlayerAbilitySet> Abilities;
	Abilities.Append(SharedAbilities);

	if (Player && Player->ActorHasTag("Player.Character.Tagger"))
		Abilities.Append(TaggerAbilities);
	if (Player && Player->ActorHasTag("Player.Character.Runner"))
		Abilities.Append(RunnerAbilities);

	return Abilities;
}