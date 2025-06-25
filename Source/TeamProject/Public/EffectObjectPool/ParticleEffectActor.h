#pragma once

#include "CoreMinimal.h"
#include "EffectObjectPool/BaseEffectActor.h"
#include "ParticleEffectActor.generated.h"

UCLASS()
class TEAMPROJECT_API AParticleEffectActor : public ABaseEffectActor
{
	GENERATED_BODY()

public:
	AParticleEffectActor();
	
public:
	UFUNCTION()
	virtual void OnParticleSystemFinished(class UParticleSystemComponent * Particle);

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent * ParticleEffect;
	
};
