// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/Tags/STGamePlayTags.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpawnerManagerSubsystem.generated.h"

/**
 * 
 */

class ABaseObjectSpawner;

UCLASS()
class TEAMPROJECT_API USpawnerManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:
	UPROPERTY()
	TArray<ABaseObjectSpawner*> RegisteredObjectSpawners;

public:
	UFUNCTION()
	void RegisterSpawner(ABaseObjectSpawner* Spawner);

	UFUNCTION()
	void UnregisterSpawner(ABaseObjectSpawner* Spawner);

	UFUNCTION()
	FGameplayTagContainer GetRegisteredSpawnerSpawnType(ABaseObjectSpawner* Spawner);
};
