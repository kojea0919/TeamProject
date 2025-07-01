#pragma once

#include "CoreMinimal.h"
#include "EffectObjectPool/BaseEffectActor.h"
#include "NiagaraEffectActor.generated.h"

UCLASS()
class TEAMPROJECT_API ANiagaraEffectActor : public ABaseEffectActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ANiagaraEffectActor();

public:
	virtual void SetEffectEnable(bool Enable) override;

	UFUNCTION()
	virtual void OnNiagaraSystemFinished(class UNiagaraComponent * Comp);
	
protected:
	UPROPERTY(VisibleAnywhere, Category = Effect, meta =(AllowPrivateAccess = true))
	class UNiagaraComponent* NiagaraComp;
	
	bool EffectEnable;
};
