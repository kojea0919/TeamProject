// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/Actor/BaseWeapon.h"
#include "BaseHammer.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseHammer : public ABaseWeapon
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
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// UStaticMeshComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UBoxComponent* WeaponCollisionBox;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<AActor*> OverlappedActors;

public:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void SetCollision(bool bIsActive);

	// UFUNCTION(BlueprintCallable, Server, Reliable)
	// void OnHammerHit(AActor* HitActor, const FHitResult& HitResult);
	// UFUNCTION(BlueprintCallable, Server, Reliable)
	// void OnHammerHitEnd(AActor* HitActor);
	// UFUNCTION()
	// void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// UFUNCTION()
	// void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_ApplyCollision(AActor* HitActor, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_PerformHammerSweep();

	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox;}
};
