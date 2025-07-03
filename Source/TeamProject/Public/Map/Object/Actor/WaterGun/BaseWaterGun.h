// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
};
