#include "EffectObjectPool/ParticleEffectActor.h"

#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
#include "Particles/ParticleSystemComponent.h"

AParticleEffectActor::AParticleEffectActor()
	: EffectEnable(false)
{
	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>("ParticleEffect");

	RootComponent = ParticleEffect;
}

void AParticleEffectActor::OnParticleSystemFinished(class UParticleSystemComponent* Particle)
{
	if (!EffectEnable)
		return;
	
	if (EffectObjPool)
	{
		//EffectObjPool->ReturnEffectObject(StaticClass(), this);			//StaticClass는 컴파일 타임의 UClass타입 정보
		EffectObjPool->ReturnEffectObject(GetClass(), this);		//GetClass는 런타임의 실제 객체의 타입 정보
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("AParticleEffectActor::OnParticleSystemFinished EffectObjPool Is Invalid"));
		Destroy();
	}
}

void AParticleEffectActor::SetEffectEnable(bool Enable)
{
	if (UseAutoReturn)
		UseTimerReturn = false;
	
	Super::SetEffectEnable(Enable);
	
	if (!ParticleEffect)
		return;

	SetActorHiddenInGame(!Enable);
	EffectEnable = Enable;
	
	if (Enable)
	{
		ParticleEffect->Activate(true);
	}
	else
		ParticleEffect->Deactivate();
}

void AParticleEffectActor::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr != ParticleEffect || UseAutoReturn)
		ParticleEffect->OnSystemFinished.AddDynamic(this,&AParticleEffectActor::OnParticleSystemFinished);
}
