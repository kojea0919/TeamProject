#include "Map/Mesh/Door/Door01.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

ADoor01::ADoor01()
{
	// 양문짝 스윙 도어로 설정
	DoorType = EDoorType::TwoDoor;
	DoorOpenType = EDoorOpenType::Swing;
	
	// 기본값 설정
	OpenAngle = 85.0f;
	DoorSpeed = 2.0f;
	
	// 문 메시 설정
	SetupDoorMeshes();
}

void ADoor01::BeginPlay()
{
	Super::BeginPlay();
	
	// 초기 회전값 저장
	if (DoorMeshComponents.Num() >= 2)
	{
		InitialRotationLeft = DoorMeshComponents[0]->GetRelativeRotation();
		InitialRotationRight = DoorMeshComponents[1]->GetRelativeRotation();
	}
}

void ADoor01::UpdateDoorAnimation(float DeltaTime)
{
	if (DoorMeshComponents.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door01: Not enough door mesh components!"));
		return;
	}

	float PreviousAlpha = CurrentAlpha;
	
	// 상태에 따른 알파값 업데이트
	switch (CurrentDoorState)
	{
		case EDoorState::Opening:
		{
			CurrentAlpha = FMath::Clamp(CurrentAlpha + DoorSpeed * DeltaTime, 0.0f, 1.0f);
			
			// 완전히 열렸을 때
			if (CurrentAlpha >= 1.0f)
			{
				SetDoorState(EDoorState::Open);
			}
			break;
		}
		
		case EDoorState::Closing:
		{
			CurrentAlpha = FMath::Clamp(CurrentAlpha - DoorSpeed * DeltaTime, 0.0f, 1.0f);
			
			// 완전히 닫혔을 때
			if (CurrentAlpha <= 0.0f)
			{
				SetDoorState(EDoorState::Closed);
			}
			break;
		}
		
		default:
			return;
	}
	
	// 알파값이 변경되었을 때만 회전 적용
	if (FMath::Abs(CurrentAlpha - PreviousAlpha) > KINDA_SMALL_NUMBER)
	{
		// 타겟 회전값 계산
		CalculateTargetRotations();
		
		// 부드러운 보간을 사용하여 회전 적용
		const float SmoothedAlpha = FMath::SmoothStep(0.0f, 1.0f, CurrentAlpha);
		
		const FRotator CurrentLeftRotation = FMath::Lerp(InitialRotationLeft, TargetRotationLeft, SmoothedAlpha);
		const FRotator CurrentRightRotation = FMath::Lerp(InitialRotationRight, TargetRotationRight, SmoothedAlpha);
		
		ApplyDoorRotation(DoorMeshComponents[0], CurrentLeftRotation);
		ApplyDoorRotation(DoorMeshComponents[1], CurrentRightRotation);
	}
}

void ADoor01::SetupDoorMeshes()
{
	// 문 메시 에셋 로드
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Game/JapaneseSchool/Meshes/WindowDoor/SM_Corridor_Door_01.SM_Corridor_Door_01")
	);

	if (!MeshAsset.Succeeded())
	{
		UE_LOG(LogTemp, Error, TEXT("Door01: Failed to load door mesh asset!"));
		return;
	}

	// 왼쪽 문 생성
	UStaticMeshComponent* LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	if (LeftDoor)
	{
		LeftDoor->SetStaticMesh(MeshAsset.Object);
		LeftDoor->SetupAttachment(Root);
		LeftDoor->SetRelativeLocationAndRotation(FVector(-100.0f, 0.0f, -10.0f), FRotator::ZeroRotator);
		LeftDoor->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		DoorMeshComponents.Add(LeftDoor);
	}

	// 오른쪽 문 생성
	UStaticMeshComponent* RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	if (RightDoor)
	{
		RightDoor->SetStaticMesh(MeshAsset.Object);
		RightDoor->SetupAttachment(Root);
		RightDoor->SetRelativeLocationAndRotation(FVector(100.0f, 0.0f, -10.0f), FRotator::ZeroRotator);
		RightDoor->SetRelativeScale3D(FVector(-1.0f, 1.0f, 1.0f)); // X축 반전으로 미러링
		DoorMeshComponents.Add(RightDoor);
	}

	// BoxCollision 설정 조정
	if (BoxCollision)
	{
		BoxCollision->SetRelativeLocationAndRotation(FVector(0.0f, -10.0f, 90.0f), FRotator::ZeroRotator);
		BoxCollision->SetRelativeScale3D(FVector(2.87262f, 3.832295f, 2.358312f));
	}
}

void ADoor01::CalculateTargetRotations()
{
	if (bOpenTowardsFront)
	{
		// 플레이어가 앞쪽에 있을 때 - 문이 플레이어 반대방향으로 열림
		TargetRotationLeft = InitialRotationLeft + FRotator(0.0f, -OpenAngle, 0.0f);
		TargetRotationRight = InitialRotationRight + FRotator(0.0f, OpenAngle, 0.0f);
	}
	else
	{
		// 플레이어가 뒤쪽에 있을 때 - 문이 플레이어 반대방향으로 열림
		TargetRotationLeft = InitialRotationLeft + FRotator(0.0f, OpenAngle, 0.0f);
		TargetRotationRight = InitialRotationRight + FRotator(0.0f, -OpenAngle, 0.0f);
	}
}