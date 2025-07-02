// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectObject/NiagaraEffect/BaseWaterGunHitEffectActor.h"

#include "NiagaraComponent.h"

ABaseWaterGunHitEffectActor::ABaseWaterGunHitEffectActor()
{
}

void ABaseWaterGunHitEffectActor::BeginPlay()
{
	Super::BeginPlay();

	NiagaraComp->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	NiagaraComp->SetRelativeScale3D(FVector(1.2f));
	
}


void ABaseWaterGunHitEffectActor::SetEffectActorSize(float NewSize)
{
	NiagaraComp->SetVariableFloat("User.size", NewSize);
}
