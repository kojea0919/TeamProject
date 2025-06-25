#include "EffectObjectPool/BaseEffectActor.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"

ABaseEffectActor::ABaseEffectActor()
	: EffectObjPool(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();

	EffectObjPool = GetWorld()->GetSubsystem<UEffectObjectPoolSubSystem>();
	checkf(EffectObjPool, TEXT("EffectObject Pool could not be found"));	
}