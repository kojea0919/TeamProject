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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing=OnMaxAmountChange)
	int MaxWaterAmount = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing=OnCurrentAmountChange)
	int CurrentWaterAmount;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetCurrentWaterAmount(int Amount);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetMaxWaterAmount(int Amount);

	UFUNCTION()
	void OnCurrentAmountChange();

	UFUNCTION()
	void OnMaxAmountChange();

	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetCurrentWaterAmount() const { return CurrentWaterAmount; }

	virtual FText GetObjectName() override;
	virtual FText GetDescription() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void InitializeObject(EGameMode GameMode) override;
};
