#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEffectActor.generated.h"

UCLASS()
class TEAMPROJECT_API ABaseEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseEffectActor();

	virtual void SetEffectEnable(bool Enable) {};
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = EffectObjectPool)
	class UEffectObjectPoolSubSystem* EffectObjPool;
};
