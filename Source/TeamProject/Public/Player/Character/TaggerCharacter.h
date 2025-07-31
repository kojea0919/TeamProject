// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/BaseCharacter.h"
#include "InputActionValue.h"
#include "TaggerCharacter.generated.h"


class ABaseWeapon;
class UTaggerRepelComponent;
class USTInputConfig;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ATaggerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	ATaggerCharacter();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* TaggerAimImg;

	virtual UTexture2D* GetAimImg() const override {return TaggerAimImg;}

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//Input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);
	void Input_StopJump(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);

	virtual bool CanCrouch() const override;
	

	virtual URepelComponent* GetRepelComponent() const override;

private:	
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta =(AllowPrivateAccess = true))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	USpringArmComponent* SpringArm;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta =(AllowPrivateAccess = true))
	// UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Repel", meta = (AllowPrivateAccess = true))
	UTaggerRepelComponent* TaggerRepelComponent;

public:
	FORCEINLINE UTaggerRepelComponent* GetTaggerRepelComponent() const { return TaggerRepelComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	ABaseObject* Hammer;

	FORCEINLINE ABaseObject* GetHammer() const { return Hammer; }
	
	
};
