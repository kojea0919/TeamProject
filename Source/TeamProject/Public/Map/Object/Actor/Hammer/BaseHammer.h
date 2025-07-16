// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/Actor/BaseObject.h"
#include "BaseHammer.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseHammer : public ABaseObject
{
	GENERATED_BODY()

	ABaseHammer();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* HammerMeshHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* HammerMeshHead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* HammerMeshTop;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* CollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<AActor*> OverlappedActors;

public:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void SetCollision(bool bIsActive);
	UFUNCTION(BlueprintCallable)
	void OnHammerHit(AActor* HitActor);
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_ApplyCollision(AActor* HitActor);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
