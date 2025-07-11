// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "Player/Character/AmimInstance/BaseAnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UCharacterAnimInstance : public UBaseAnimInstance
{
	
	GENERATED_BODY()


protected:
	// 초기값 설정
	virtual void NativeInitializeAnimation() override;

	// 업데이트시 최적화를 시키는 함수
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	class ABaseCharacter* OwningCharacter;

	UPROPERTY()
	class UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	float GroundSpeed;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	bool bHasAccelerationCode;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	float LocomotionDirection;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true));
	bool bIsAiming;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bIsSprinting;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bIsCrouching;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bIsEquipping;
		

	
	
};
