// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/Actor/Door/BaseDoor.h"
#include "JailDoor.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AJailDoor : public ABaseDoor
{
	GENERATED_BODY()
	AJailDoor();

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings", meta = (ClampMin = "30.0", ClampMax = "120.0"))
	float OpenAngle = 85.0f;
	
	FRotator TargetRotation;
	
	// 초기 회전값 (닫힌 상태)
	FRotator InitialRotation;
	
	virtual void SetupDoorMeshes() override;
	virtual void CalculateTargetRotations() override;

	virtual void ApplyDoorAnimation(float Alpha) override;
};
