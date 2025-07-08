#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Map/Object/Actor/BaseObject.h"
#include "Player/Character/BaseCharacter.h"
#include "BaseEffectActor.generated.h"

UENUM(BlueprintType)
enum class EEffectReturnType : uint8
{
	SelfReturn,
	AutoReturn,
	TimerReturn
};

UCLASS()
class TEAMPROJECT_API ABaseEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseEffectActor();

	virtual void SetEffectEnable(bool Enable);
	void DeativeEffect();

protected:
	virtual void BeginPlay() override;

protected:
	void ReturnToObjectPool();

public:
	UFUNCTION(BlueprintCallable)
    virtual void EffectSetUp(const ABaseCharacter* Player, const ABaseObject* Object) {}
	
protected:

	
	UPROPERTY(EditAnywhere, Category = EffectObjectPool)
	EEffectReturnType EffectReturnType;
	
	UPROPERTY(EditAnywhere, Category = EffectObjectPool)
	float EffectRunningTime;
	
	UPROPERTY(VisibleAnywhere, Category = EffectObjectPool, meta=(AllowPrivateAccess = true))
	class UEffectObjectPoolSubSystem* EffectObjPool;
	
	FTimerHandle ReturnTimerHandle;	
};
