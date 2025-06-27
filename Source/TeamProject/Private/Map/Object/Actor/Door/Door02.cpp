// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/Door/Door02.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

ADoor02::ADoor02()
{
	// 양문짝 스윙 도어로 설정
	DoorType = EDoorType::TwoDoor;
	DoorOpenType = EDoorOpenType::Swing;
	
	// 위치가 변하지 않는 문이므로 위치 복제 비활성화
	SetReplicateMovement(false);
	
	// 기본값 설정
	OpenAngle = 85.0f;
	DoorSpeed = 2.0f;
	
	// 문 메시 설정
	SetupDoorMeshes();
}

void ADoor02::BeginPlay()
{
	Super::BeginPlay();
	
	// 초기 회전값 저장
	if (DoorMeshComponents.Num() >= 2)
	{
		InitialRotation = DoorMeshComponents[0]->GetRelativeRotation();
	}
}

void ADoor02::ApplyDoorAnimation(float Alpha)
{
	if (DoorMeshComponents.Num() < 1)
	{
		return;
	}

	// 타겟 회전값 계산
	CalculateTargetRotations();
	
	// 부드러운 보간을 사용하여 회전 적용
	const float SmoothedAlpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);
	
	const FRotator CurrentRotation = FMath::Lerp(InitialRotation, TargetRotation, SmoothedAlpha);
	
	ApplyDoorRotation(DoorMeshComponents[0], CurrentRotation);

}

void ADoor02::SetupDoorMeshes()
{
	// 문 메시 에셋 로드
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Game/JapaneseSchool/Meshes/WindowDoor/SM_Toilet_Door_01.SM_Toilet_Door_01")
	);

	if (!MeshAsset.Succeeded())
	{
		return;
	}

	// 왼쪽 문 생성
	UStaticMeshComponent* Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	if (Door)
	{
		Door->SetStaticMesh(MeshAsset.Object);
		Door->SetupAttachment(Root);
		Door->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator);
		Door->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		DoorMeshComponents.Add(Door);
	}

	// BoxCollision 설정 조정
	if (BoxCollision)
	{
		BoxCollision->SetRelativeLocationAndRotation(FVector(-50.0f, 0.f, 90.0f), FRotator::ZeroRotator);
		BoxCollision->SetRelativeScale3D(FVector(1.5775f, 3.832295f, 1.858312f));
	}
}

void ADoor02::CalculateTargetRotations()
{
	if (bOpenTowardsFront)
	{
		// 플레이어가 앞쪽에 있을 때 - 문이 플레이어 반대방향으로 열림
		TargetRotation = InitialRotation + FRotator(0.0f, OpenAngle, 0.0f);
	}
	else
	{
		// 플레이어가 뒤쪽에 있을 때 - 문이 플레이어 반대방향으로 열림
		TargetRotation = InitialRotation + FRotator(0.0f, -OpenAngle, 0.0f);
	}
}