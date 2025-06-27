// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectObjectPool/NiagaraEffectActor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"

ANiagaraEffectActor::ANiagaraEffectActor()
	: NiagaraEffect(nullptr),NiagaraComp(nullptr), EffectEnable(false), UseAutoReturn(false)
{
}

void ANiagaraEffectActor::SpawnNiagaraEffect(const FTransform& Transform)
{
	if ( nullptr == NiagaraEffect)
	{
		UE_LOG(LogTemp,Warning,TEXT("ANiagaraEffectActor::SpwanNiagaraEffect NiagaraEffect Null"));
		return;
	}
	
	if (nullptr == NiagaraComp)
	{
		NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, Transform.GetLocation(),
			Transform.GetRotation().Rotator(), FVector(1.0f), false); //재사용을 위한 bAutoDestory false

		RootComponent = NiagaraComp;

		if (UseAutoReturn)
			NiagaraComp->OnSystemFinished.AddDynamic(this ,&ANiagaraEffectActor::OnNiagaraSystemFinished);
	}
	else
	{
		NiagaraComp->SetWorldLocationAndRotation(Transform.GetLocation(), Transform.GetRotation().Rotator());
	}
}

void ANiagaraEffectActor::SpawnAndAttachNiagaraEffect(class USkeletalMeshComponent* TargetMesh, FName SocketName)
{
	if (nullptr == NiagaraEffect)
	{
		UE_LOG(LogTemp,Warning,TEXT("ANiagaraEffectActor::SpawnAndAttachNiagaraEffect NiagaraEffect Null"));
		return;
	}

	if (nullptr == NiagaraComp)
	{
		FTransform SocketTransform = TargetMesh->GetSocketTransform(SocketName);
		NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraEffect, TargetMesh, SocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false);

		RootComponent = NiagaraComp;
		NiagaraComp->AttachToComponent(TargetMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

		if (UseAutoReturn)
			NiagaraComp->OnSystemFinished.AddDynamic(this ,&ANiagaraEffectActor::OnNiagaraSystemFinished);
	}
	else
	{
		NiagaraComp->AttachToComponent(TargetMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
}

void ANiagaraEffectActor::SetEffectEnable(bool Enable)
{
	if (UseAutoReturn)
		UseTimerReturn = false;
	
	Super::SetEffectEnable(Enable);	
	
	EffectEnable = Enable;
	if (nullptr == NiagaraComp)
		return;
	
	if (Enable)
	{
		NiagaraComp->ResetSystem();
		NiagaraComp->Activate(true);
	}
	else
	{
		NiagaraComp->Deactivate();
		NiagaraComp->DeactivateImmediate();
	}
}

void ANiagaraEffectActor::OnNiagaraSystemFinished(class UNiagaraComponent* Comp)
{
	if (!EffectEnable)
		return;
	
	if (EffectObjPool)
	{
		EffectObjPool->ReturnEffectObject(GetClass(), this);		//GetClass는 런타임의 실제 객체의 타입 정보
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("ANiagaraEffectActor::OnNiagaraSystemFinished EffectObjPool Is Invalid"));
		Destroy();
	}	
}
