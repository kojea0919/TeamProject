#include "EffectObjectPool/BaseEffectActor.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"

ABaseEffectActor::ABaseEffectActor()
	: EffectRunningTime(1), EffectObjPool(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseEffectActor::SetEffectEnable(bool Enable)
{
	if (Enable)
	{
		if (EffectReturnType == EEffectReturnType::TimerReturn)
		{
			GetWorldTimerManager().SetTimer(ReturnTimerHandle, this,
				&ABaseEffectActor::ReturnToObjectPool,EffectRunningTime, false);			
		}
	}
}

void ABaseEffectActor::DeativeEffect()
{
	if (EffectReturnType == EEffectReturnType::SelfReturn)
		ReturnToObjectPool();
}

void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();

	EffectObjPool = GetWorld()->GetSubsystem<UEffectObjectPoolSubSystem>();
	checkf(EffectObjPool, TEXT("EffectObject Pool could not be found"));	
}

void ABaseEffectActor::ReturnToObjectPool()
{
	checkf(EffectObjPool , TEXT("ABaseEffectActor::ReturnToObjectPool EffectObjPool Null"));

	EffectObjPool->ReturnEffectObject(GetClass(), this);
}
