// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/Component/BaseObjectComponent.h"
#include "WaterSplashObjectComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UWaterSplashObjectComponent : public UBaseObjectComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void OnHitBySplash(AActor* HitActor);
};
