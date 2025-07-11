// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/BaseObject.h"

#include "EffectObjectPool/BaseEffectActor.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Map/Object/AbilitySystem/BaseObjectGameplayAbility.h"
#include "Player/Character/Component/Interactive/RunnerInterActiveComponent.h"

// Sets default values
ABaseObject::ABaseObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
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


void ABaseObject::SetEffectActorTransform(ABaseEffectActor* EffectActor, FTransform Transform)
{
	if (EffectActor == nullptr)
		return;

	EffectActor->SetActorTransform(Transform);
}

void ABaseObject::GetEffectObjectFromPool_Implementation(ABaseCharacter* Player, TSubclassOf<ABaseEffectActor> EffectActorClass,
                                                         const FTransform& Transform)
{
	UEffectObjectPoolSubSystem* ObjectPool = GetWorld()->GetSubsystem<UEffectObjectPoolSubSystem>();

	ABaseEffectActor* SpawnedEffect = ObjectPool->GetEffectObject(EffectActorClass);

	SetEffectActorTransform(SpawnedEffect, Transform);
	SpawnedEffect->EffectSetUp(Player, this);
}
