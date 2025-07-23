// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/Actor/BaseObject.h"
#include "BaseWeapon.generated.h"

DECLARE_DELEGATE_TwoParams(FOnTargetInteractedDelegate, AActor*, const FHitResult&);
DECLARE_DELEGATE_OneParam(FOnWeaponHitTargetEndDelegate, AActor*)

class UBoxComponent;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseWeapon : public ABaseObject
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	FOnTargetInteractedDelegate OnWeaponHitTarget;
	FOnWeaponHitTargetEndDelegate OnWeaponPulledFromTarget;

protected:
	


public:
	

	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	
};
