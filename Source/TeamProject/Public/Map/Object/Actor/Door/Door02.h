// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/Actor/Door/BaseDoor.h"
#include "Door02.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ADoor02 : public ABaseDoor
{
	GENERATED_BODY()

public:
	ADoor02();

protected:
	virtual void BeginPlay() override;
	
	// 실제 애니메이션 구현 (BaseDoor에서 호출)
	virtual void ApplyDoorAnimation(float Alpha) override;

protected:
	// 문 열림 각도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings", meta = (ClampMin = "30.0", ClampMax = "120.0"))
	float OpenAngle = 85.0f;

	// 문의 회전 값
	FRotator TargetRotation;
	
	// 초기 회전값
	FRotator InitialRotation;

	// 문 메시 컴포넌트 생성 및 설정
	virtual void SetupDoorMeshes() override;
	
	// 타겟 회전값 계산
	virtual void CalculateTargetRotations() override;

	virtual void InitializeObject(EGameMode GameMode) override;
};