// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectObject/NiagaraEffect/BaseWaterGunHitEffectActor.h"

#include "NiagaraComponent.h"

ABaseWaterGunHitEffectActor::ABaseWaterGunHitEffectActor()
{

}

void ABaseWaterGunHitEffectActor::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	NiagaraComp->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	NiagaraComp->SetRelativeScale3D(FVector(1.2f));

	SetActorTickEnabled(false);
}

void ABaseWaterGunHitEffectActor::EffectSetUp(const ABaseCharacter* Player, const ABaseObject* Object)
{
	SetActorTickEnabled(true);
}

void ABaseWaterGunHitEffectActor::EffectSetActive(bool IsActive)
{
	SetActorHiddenInGame(!IsActive);         // 화면에서 숨기기
	SetActorEnableCollision(IsActive);     // 충돌 끄기
	SetActorTickEnabled(IsActive); 
}


void ABaseWaterGunHitEffectActor::SetEffectActorSize(float NewSize)
{
	NiagaraComp->SetVariableFloat("User.size", NewSize);
}

void ABaseWaterGunHitEffectActor::ReturnEffectActor()
{
	//Destroy();
	ReturnToObjectPool();
}
