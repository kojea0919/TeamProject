// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/BaseObject.h"

#include "Map/Object/AbilitySystem/BaseObjectGameplayAbility.h"
#include "Player/Character/Component/Interactive/RunnerInterActiveComponent.h"

// Sets default values
ABaseObject::ABaseObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	
}

void ABaseObject::SetGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles)
{
	GrantedAbilitySpecHandles = SpecHandles;
}

TArray<FGameplayAbilitySpecHandle> ABaseObject::GetGrantedAbilitySpecHandles() const 
{
	return GrantedAbilitySpecHandles;
}

UPawnInterActiveComponent* ABaseObject::GetInterActiveComponent() const
{
	return RunnerInterActiveComponent;
}

