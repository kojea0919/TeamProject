// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectObjectPool/NiagaraEffectActor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"


ANiagaraEffectActor::ANiagaraEffectActor()
	: NiagaraComp(nullptr), EffectEnable(false)
{
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));

	RootComponent = NiagaraComp;
}

void ANiagaraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	if (NiagaraComp != nullptr)
	{
		if (nullptr != NiagaraComp->GetAsset() || EffectReturnType == EEffectReturnType::AutoReturn)
			NiagaraComp->OnSystemFinished.AddDynamic(this ,&ANiagaraEffectActor::OnNiagaraSystemFinished);
	}
}

void ANiagaraEffectActor::SetEffectEnable(bool Enable)
{	
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
