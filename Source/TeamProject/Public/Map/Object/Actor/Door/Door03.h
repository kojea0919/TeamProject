// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/Actor/Door/BaseDoor.h"
#include "Door03.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ADoor03 : public ABaseDoor
{
	GENERATED_BODY()

public:
	ADoor03();

protected:
	virtual void BeginPlay() override;
	
	// 실제 애니메이션 구현 (BaseDoor에서 호출)
	virtual void ApplyDoorAnimation(float Alpha) override;

protected:
	// 문 열림 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings", meta = (ClampMin = "30.0", ClampMax = "120.0"))
	float OpenDistance = 90.0f;

	// 문의 위치 값
	FVector TargetLocation;
	
	// 초기 위치 값
	FVector InitialLocation;

	// 문 메시 컴포넌트 생성 및 설정
	virtual void SetupDoorMeshes() override;
	
	// 타겟 회전값 계산
	virtual void CalculateTargetLocations() override;

	virtual void InitializeGameStart(EGameMode GameMode) override;
};