// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectObjectPool/NiagaraEffectActor.h"
#include "BaseWaterGunBeamEffectActor.generated.h"

class ABaseWaterGunHitEffectActor;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseWaterGunBeamEffectActor : public ANiagaraEffectActor
{
	GENERATED_BODY()

public:
	ABaseWaterGunBeamEffectActor();

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
#pragma region EffectSetting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	float BeamLength = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	float SizeCorrect = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	float BeamLengthBck = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	float Duration = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	float BeamSpeed = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	float ReturnSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	bool bIsLoop = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	AActor* BeamStartActor = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	AActor* BeamEndActor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	FVector BeamStartOffset = FVector(0.0f);
#pragma endregion
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hit Effect")
	TSubclassOf<ABaseWaterGunHitEffectActor> HitEffectActor;
	
	UPROPERTY()
	ABaseWaterGunHitEffectActor* HitEffectActorInstance = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FTimerHandle LoopTimerHandle;
	
	UPROPERTY(EditDefaultsOnly)
	bool bIsBeamShot = false;

	UFUNCTION()
	void StartLoop();

	UFUNCTION()
	void LoopExecution();

	UFUNCTION()
	void FinishLoop();
	
	virtual void EffectSetUp() override;
	
	UFUNCTION(Server, Reliable)
	void CheckCollision();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyCollision(FHitResult OutResult);

	UFUNCTION()
	void BeamControl(float NewBeamLength);

	UFUNCTION()
	void SetHitEffectActive(bool IsActive);
};
