#pragma once

#include "CoreMinimal.h"
#include "EffectObjectPool/BaseEffectActor.h"
#include "NiagaraEffectActor.generated.h"

UCLASS()
class TEAMPROJECT_API ANiagaraEffectActor : public ABaseEffectActor
{
	GENERATED_BODY()


public:
	ANiagaraEffectActor();

public:
	UFUNCTION(BlueprintCallable)
	virtual void SpawnNiagaraEffect(const FTransform& Transform);

	UFUNCTION(BlueprintCallable)
	virtual void SpawnAndAttachNiagaraEffect(class USkeletalMeshComponent* TargetMesh, FName SocketName);

	virtual void SetEffectEnable(bool Enable) override;

	UFUNCTION()
	virtual void OnNiagaraSystemFinished(class UNiagaraComponent * Comp);
	
protected:
	UPROPERTY(EditAnywhere, Category = Effect)
	class UNiagaraSystem* NiagaraEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	class UNiagaraComponent* NiagaraComp;
	
	bool EffectEnable;

	UPROPERTY(EditAnywhere, Category = EffectObjectPool)
	bool UseAutoReturn;
};
