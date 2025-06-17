#pragma once

#include "CoreMinimal.h"
#include "Map/Mesh/Door/BaseDoor.h"
#include "Door01.generated.h"

/**
 * 스윙 방식 양문짝 문 클래스
 */
UCLASS()
class TEAMPROJECT_API ADoor01 : public ABaseDoor
{
	GENERATED_BODY()

public:
	ADoor01();

protected:
	virtual void BeginPlay() override;
	
	// 실제 애니메이션 구현 (BaseDoor에서 호출)
	virtual void ApplyDoorAnimation(float Alpha) override;

protected:
	// 문 열림 각도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings", meta = (ClampMin = "30.0", ClampMax = "120.0"))
	float OpenAngle = 85.0f;

	// 왼쪽 문과 오른쪽 문의 타겟 회전값
	FRotator TargetRotationLeft;
	FRotator TargetRotationRight;
	
	// 초기 회전값 (닫힌 상태)
	FRotator InitialRotationLeft;
	FRotator InitialRotationRight;

	// 문 메시 컴포넌트 생성 및 설정
	void SetupDoorMeshes();
	
	// 타겟 회전값 계산
	virtual void CalculateTargetRotations() override;
};