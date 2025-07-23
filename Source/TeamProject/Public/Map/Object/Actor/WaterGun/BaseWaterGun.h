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
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WaterGunMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* NozzleLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ShootAngleLocation;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterAmount", meta=(AllowPrivateAccess))
	int MaxWaterAmount = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaterAmount")
	int CurrentWaterAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterAmount", meta=(AllowPrivateAccess))
	int WaterAmountPerShot = 0;

public:
	UFUNCTION(BlueprintPure)
	FRotator GetShootAngle();

	UFUNCTION()
	FORCEINLINE USceneComponent* GetNozzleLocation() const { return NozzleLocation; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetMaxWaterAmount() const { return MaxWaterAmount; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetCurrentWaterAmount() const { return CurrentWaterAmount; };

	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetWaterAmountPerShot() const { return WaterAmountPerShot; };

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetCurrentWaterAmount(int Amount);
	
	virtual FText GetObjectName() override;
	
	virtual FText GetDescription() override;
};
