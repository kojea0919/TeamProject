// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/BaseObjectSpawner.h"

#include "Map/Object/Subsystem/WorldSubsystem/SpawnerManagerSubsystem.h"

ABaseObjectSpawner::ABaseObjectSpawner()
{
}

void ABaseObjectSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
		RegisterToSpawnManager();
}

void ABaseObjectSpawner::EndPlay(const EEndPlayReason::Type reason)
{
	Super::EndPlay(reason);

	if (HasAuthority())
		UnregisterFromSpawnManager();
}

bool ABaseObjectSpawner::RegisterToSpawnManager()
{
	if (bIsRegistered)
		return false;

	USpawnerManagerSubsystem* SpawnManager = GetWorld()->GetSubsystem<USpawnerManagerSubsystem>();
	if (!SpawnManager)
		return false;

	SpawnManager->RegisterSpawner(this);
	UpdateRegistrationStatus(true);
	return true;
}

bool ABaseObjectSpawner::UnregisterFromSpawnManager()
{
	if (!bIsRegistered)
	{
		return false;
	}

	USpawnerManagerSubsystem* SpawnManager = GetWorld()->GetSubsystem<USpawnerManagerSubsystem>();
	if (!SpawnManager)
	{
		return false;
	}
	
	SpawnManager->UnregisterSpawner(this);
	UpdateRegistrationStatus(false);

	return true;
}

FGameplayTagContainer ABaseObjectSpawner::GetSpawnTypes()
{
	return SpawnObjectTypes;
}


void ABaseObjectSpawner::UpdateRegistrationStatus(const bool bNewStatus)
{
	bIsRegistered = bNewStatus;
}

