// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/Component/Repel/RepelComponent.h"
#include "TaggerRepelComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UTaggerRepelComponent : public URepelComponent
{
	GENERATED_BODY()

public:

	UTaggerRepelComponent();
	
	UFUNCTION(BlueprintPure, Category = "Repel")
	ABaseWeapon* GetTaggerCarriedWeaponByTag(FGameplayTag Tag);
	
	
	virtual void OnHitTargetActor(AActor* HitActor, const FHitResult& HitResult) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;

	UFUNCTION(BlueprintCallable, Category = "Repel")
	ABaseWeapon* GetTaggerCurrentEquippedWeapon() const;

	UFUNCTION(Server, Reliable)
	void Server_SendHammerHitEvent(AActor* HitActor, const FHitResult& HitResult);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyHammerHit(AActor* HitActor, const FHitResult& HitResult);

	void ApplyHammerHit(AActor* HitActor, const FHitResult& HitResult);
	
	
};
