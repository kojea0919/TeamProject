#pragma once

#include "CoreMinimal.h"
#include "Map/Mesh/BaseObject.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "BaseDoor.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum class EDoorType : uint8
{
	OneDoor UMETA(DisplayName = "One Door"),
	TwoDoor UMETA(DisplayName = "Two Door")
};

UENUM(BlueprintType)
enum class EDoorOpenType : uint8
{
	Slide UMETA(DisplayName = "Slide"),
	Swing UMETA(DisplayName = "Swing")
};

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Closed UMETA(DisplayName = "Closed"),
	Opening UMETA(DisplayName = "Opening"),
	Open UMETA(DisplayName = "Open"),
	Closing UMETA(DisplayName = "Closing")
};

UCLASS()
class TEAMPROJECT_API ABaseDoor : public ABaseObject
{
	GENERATED_BODY()
	
public:	
	ABaseDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	// 네트워크 복제 설정
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Data")
	TArray<UStaticMeshComponent*> DoorMeshComponents;
	
	// 문 개수 타입 (한쪽 문/양쪽 문)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Data")
	EDoorType DoorType = EDoorType::OneDoor;

	// 문 여는 방식 (슬라이드/스윙)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Data")
	EDoorOpenType DoorOpenType = EDoorOpenType::Swing;

	// 문 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings", meta = (ClampMin = "0.5", ClampMax = "10.0"))
	float DoorSpeed = 2.0f;

	// 문 상태 (네트워크 복제)
	UPROPERTY(ReplicatedUsing = OnRep_DoorState, VisibleAnywhere, BlueprintReadOnly, Category = "Door State")
	EDoorState CurrentDoorState = EDoorState::Closed;
	
	// 현재 애니메이션 진행도 (네트워크 복제) (0.0 = 완전 닫힘, 1.0 = 완전 열림)
	UPROPERTY(ReplicatedUsing = OnRep_DoorAlpha, VisibleAnywhere, BlueprintReadOnly, Category = "Door State")
	float CurrentAlpha = 0.0f;
	
	// 문 열림 방향 (네트워크 복제) (true = 앞쪽, false = 뒤쪽)
	UPROPERTY(Replicated)
	bool bOpenTowardsFront = false;
	
	// 현재 overlap 중인 액터 수 (서버에서만 관리)
	int32 OverlappingActorCount = 0;

	// 문 열기 방향 계산
	bool FindDoorOpenDirection(const AActor* PlayerActor) const;

	// 문 상태 변경 (서버에서만 실행)
	UFUNCTION(BlueprintCallable, Category = "Door")
	virtual void SetDoorState(EDoorState NewState);

	// 문 열기/닫기 (서버에서만 실행)
	UFUNCTION(BlueprintCallable, Category = "Door")
	virtual void OpenDoor();
	
	UFUNCTION(BlueprintCallable, Category = "Door")
	virtual void CloseDoor();

	// Alpha 값 업데이트 및 애니메이션 관리 (BaseDoor에서 처리)
	virtual void UpdateDoorAnimation(float DeltaTime);

	// 실제 애니메이션 구현 (자식 클래스에서 구현)
	virtual void ApplyDoorAnimation(float Alpha) {}

	// Overlap 이벤트 (서버에서만 처리)
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 문 회전 적용 (유틸리티 함수)
	UFUNCTION(BlueprintCallable, Category = "Door")
	virtual void ApplyDoorRotation(UStaticMeshComponent* Door, const FRotator& Rotation);

	// 유효한 액터인지 확인
	virtual bool IsValidOverlappingActor(AActor* Actor) const;

	// 네트워크 복제 함수들
	UFUNCTION()
	virtual void OnRep_DoorState();
	
	UFUNCTION()
	virtual void OnRep_DoorAlpha();

	// 서버 RPC - 문 상태 변경 요청
	UFUNCTION(Server, Reliable)
	void ServerSetDoorState(EDoorState NewState, bool bNewOpenDirection);

private:
	// 이전 Alpha 값 (변화 감지용)
	float PreviousAlpha = 0.0f;
};