// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Subsystem/WorldSubsystem/SpawnerManagerSubsystem.h"

#include "Map/Object/BaseObjectSpawner.h"


void USpawnerManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RegisteredObjectSpawners.Empty();

	UE_LOG(LogTemp, Warning, TEXT("Spawner Manager Initialized"));
}

void USpawnerManagerSubsystem::Deinitialize()
{
	RegisteredObjectSpawners.Empty();
	
	Super::Deinitialize();
}

void USpawnerManagerSubsystem::RegisterSpawner(ABaseObjectSpawner* Spawner)
{
	check(Spawner);

	if (RegisteredObjectSpawners.Contains(Spawner))
		return;

	RegisteredObjectSpawners.Add(Spawner);
}

void USpawnerManagerSubsystem::UnregisterSpawner(ABaseObjectSpawner* Spawner)
{
	check(Spawner);

	if (RegisteredObjectSpawners.Contains(Spawner))
		RegisteredObjectSpawners.Remove(Spawner);
}

FGameplayTagContainer USpawnerManagerSubsystem::GetRegisteredSpawnerSpawnType(ABaseObjectSpawner* Spawner)
{
	return Spawner->GetSpawnTypes();
}