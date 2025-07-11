// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/Component/Repel/RepelComponent.h"
#include "TaggerRepelComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UTaggerRepelComponent : public URepelComponent
{
	GENERATED_BODY()

public:
	virtual void OnHitTargetActor(AActor* HitActor) override;
	
};
