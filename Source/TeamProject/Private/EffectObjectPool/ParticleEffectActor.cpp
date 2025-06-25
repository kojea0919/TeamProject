#include "EffectObjectPool/ParticleEffectActor.h"

#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
#include "Particles/ParticleSystemComponent.h"

AParticleEffectActor::AParticleEffectActor()
{
	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>("ParticleEffect");
	
}

void AParticleEffectActor::OnParticleSystemFinished(class UParticleSystemComponent* Particle)
{
	if (EffectObjPool)
	{
		EffectObjPool->ReturnEffectObject(StaticClass(), this);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("AParticleEffectActor::OnParticleSystemFinished EffectObjPool Is Invalid"));
		Destroy();
	}
}

void AParticleEffectActor::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr != ParticleEffect)
		ParticleEffect->OnSystemFinished.AddDynamic(this,&AParticleEffectActor::OnParticleSystemFinished);
}
