// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Map/Object/Actor/BaseObject.h"
#include "BaseWaterGun.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseWaterGun : public ABaseObject
{
	GENERATED_BODY()

public:
	ABaseWaterGun();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WaterGunMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* NozzleLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ShootAngleLocation;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaterAmount")
	float MaxWaterAmount = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaterAmount")
	float CurrentWaterAmount = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaterAmount")
	float WaterAmountPerShot = 0;

public:
	UFUNCTION(BlueprintPure)
	FRotator GetShootAngle();

	UFUNCTION()
	FORCEINLINE USceneComponent* GetNozzleLocation() const { return NozzleLocation; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxWaterAmount() const { return MaxWaterAmount; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentWaterAmount() const { return CurrentWaterAmount; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetWaterAmountPerShot() const { return WaterAmountPerShot; }

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	FORCEINLINE void SetCurrentWaterAmount(float Amount);
};
