// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/TargetPoint.h"
#include "BaseObjectSpawner.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseObjectSpawner : public ATargetPoint
{
	GENERATED_BODY()

public:
	ABaseObjectSpawner();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type reason) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner Setting", meta=(Categories="Object"))
	FGameplayTagContainer SpawnObjectTypes;

public:
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	bool RegisterToSpawnManager();

	// 수동으로 SpawnManager에서 해제
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	bool UnregisterFromSpawnManager();

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FGameplayTagContainer GetSpawnTypes();


protected:
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void UpdateRegistrationStatus(const bool bNewStatus);

private:
	bool bIsRegistered = false;
};
