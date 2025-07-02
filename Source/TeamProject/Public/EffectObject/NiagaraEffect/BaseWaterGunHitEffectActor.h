// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectObjectPool/NiagaraEffectActor.h"
#include "BaseWaterGunHitEffectActor.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseWaterGunHitEffectActor : public ANiagaraEffectActor
{
	GENERATED_BODY()

public:
	ABaseWaterGunHitEffectActor();

	virtual void BeginPlay() override;

	void SetEffectActorSize(float NewSize);
};
