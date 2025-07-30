// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectObjectPool/NiagaraEffectActor.h"
#include "BaseHammerHitEffectActor.generated.h"

class UGC_BaseSound;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseHammerHitEffectActor : public ANiagaraEffectActor
{
	GENERATED_BODY()

public:
	virtual void EffectSetUp(const ABaseCharacter* Player, const ABaseObject* Object) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess))
	UGC_BaseSound* HitSound;
};
