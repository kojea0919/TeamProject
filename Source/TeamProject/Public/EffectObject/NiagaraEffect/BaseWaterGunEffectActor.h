// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectObjectPool/NiagaraEffectActor.h"
#include "BaseWaterGunEffectActor.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseWaterGunEffectActor : public ANiagaraEffectActor
{
	GENERATED_BODY()

public:
	ABaseWaterGunEffectActor();

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

#pragma region Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Root Component")
	USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Source")
	UNiagaraComponent* BeamComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Source")
	UNiagaraComponent* HitComponent;
#pragma endregion
	
#pragma region EffectSetting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Setting")
	float BeamLength = 35.0f;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTimerHandle LoopTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsBeamShot = false;

	UFUNCTION()
	void StartLoop();

	UFUNCTION()
	void LoopExecution();

	UFUNCTION()
	void FinishLoop();
	
	UFUNCTION()
	FHitResult CheckCollision() const;

	UFUNCTION()
	void BeamControl(float NewBeamLength);
};
