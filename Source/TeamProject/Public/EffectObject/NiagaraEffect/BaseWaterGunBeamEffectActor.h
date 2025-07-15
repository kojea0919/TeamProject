// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectObjectPool/NiagaraEffectActor.h"
#include "BaseWaterGunBeamEffectActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSplashHIt, AActor*, HitActor);

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

	static FOnSplashHIt OnSplashHit;

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
	float BeamStartMoveSpeed = 1500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	float BeamEndMoveSpeed = 1500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	float MeetDistance = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	AActor* BeamStartActor = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	AActor* BeamEndActor = nullptr;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	//FVector BeamStartOffset = FVector(0.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	FVector BeamDirectionNormal = FVector::ZeroVector;
#pragma endregion
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hit Effect")
	TSubclassOf<ABaseWaterGunHitEffectActor> HitEffectActor;
	
	UPROPERTY()
	ABaseWaterGunHitEffectActor* HitEffectActorInstance = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FTimerHandle LoopTimerHandle;
	
	UPROPERTY(EditDefaultsOnly)
	bool bIsBeamShot = false;

	UFUNCTION(Server, Reliable)
	void CheckDestroy();

	UFUNCTION(NetMulticast, Reliable)
	void FinishLoop();
	
	virtual void EffectSetUp(const ABaseCharacter* Player, const ABaseObject* Object) override;
	
	UFUNCTION(Server, Reliable)
	void CheckCollision();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyCollision(FHitResult OutResult);

	UFUNCTION()
	void BeamControl(float NewBeamLength);

	UFUNCTION()
	void SetHitEffectActive(bool IsActive);

public:
	UFUNCTION(Server, Reliable)
	void UpdateBeamPosition();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyBeamPosition(FVector LocalStartPosition, FVector LocalEndPosition);

private:
	TWeakObjectPtr<ABaseObject> CachedObject;
	TWeakObjectPtr<ABaseCharacter> CachedCharacter;
	FVector StartPosition;
};
