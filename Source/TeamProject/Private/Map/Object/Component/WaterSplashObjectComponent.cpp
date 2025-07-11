// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Component/WaterSplashObjectComponent.h"

#include "EffectObject/NiagaraEffect/BaseWaterGunBeamEffectActor.h"

void UWaterSplashObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	ABaseWaterGunBeamEffectActor::OnSplashHit.AddDynamic(this, &ThisClass::OnHitBySplash);
}

void UWaterSplashObjectComponent::OnHitBySplash(AActor* HitActor)
{
	
}
