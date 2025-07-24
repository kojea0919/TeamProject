// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/BaseCharacter.h"
#include "InputActionValue.h"
#include "GameFrameWork/MainMap/StaticMeshManager/StaticMeshManageList.h"
#include "RunnerCharacter.generated.h"


class USTInputConfig;
class UCameraComponent;
class USpringArmComponent;
class URunnerRepelComponent;
class URunnerInterActiveComponent;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ARunnerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	ARunnerCharacter();

public:
	UFUNCTION(Server, Reliable)
	void SetCurrentObjectType(EStaticMeshType MeshType);

	UFUNCTION()
	void OnRep_ObjectType();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void SetGhostMode();

	UFUNCTION(Client, Reliable)
	void SetOutLine(const TArray<ARunnerCharacter*> & OutlineTargets, bool Active);


	
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

	// RepelComponent
	virtual URepelComponent* GetRepelComponent() const override;

	// InterActiveComponent
	virtual UPawnInterActiveComponent* GetInterActiveComponent() const override;

	

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ObjectType, meta = (AllowPrivateAccess = true))
	EStaticMeshType CurrentObjectType = EStaticMeshType::None;

private:
#pragma region component
	
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta =(AllowPrivateAccess = true))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	USpringArmComponent* SpringArm;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta =(AllowPrivateAccess = true))
	// UCameraComponent* FollowCamera;

	// RepelComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Repel", meta = (AllowPrivateAccess = true))
	URunnerRepelComponent* RunnerRepelComponent;

	// InteractiveComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InterActive", meta =(AllowPrivateAccess = true))
	URunnerInterActiveComponent* RunnerInterActiveComponent;

	//StaticMeshComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMesh", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent * StaticMesh;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	USTInputConfig* InputConfigDataAsset;

public:
	FORCEINLINE URunnerRepelComponent* GetRunnerRepelComponent() const { return RunnerRepelComponent; }
	FORCEINLINE URunnerInterActiveComponent* GetRunnerInterActiveComponent() const { return RunnerInterActiveComponent; }
#pragma endregion

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Test();
};
