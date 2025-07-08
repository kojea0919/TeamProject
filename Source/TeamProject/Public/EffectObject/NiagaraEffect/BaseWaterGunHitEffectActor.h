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

	virtual void EffectSetUp(const ABaseCharacter* Player, const ABaseObject* Object) override;

	void EffectSetActive(bool IsActive);

	void SetEffectActorSize(float NewSize);
	
	void ReturnEffectActor();
};
