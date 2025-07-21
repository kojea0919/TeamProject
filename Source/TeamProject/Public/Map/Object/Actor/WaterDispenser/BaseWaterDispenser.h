// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/Actor/BaseObject.h"
#include "BaseWaterDispenser.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseWaterDispenser : public ABaseObject
{
	GENERATED_BODY()

public:
	ABaseWaterDispenser();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CurrentWaterAmount = 10;

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetCurrentWaterAmount(int Amount);

	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetCurrentWaterAmount() const { return CurrentWaterAmount; }
};
