// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Mesh/Door/Door03.h"
#include "Components/BoxComponent.h"

ADoor03::ADoor03()
{
	// 양문짝 스윙 도어로 설정
	DoorType = EDoorType::TwoDoor;
	DoorOpenType = EDoorOpenType::Slide;
	
	// 기본값 설정
	OpenDistance = 90.0f;
	DoorSpeed = 2.0f;
	
	// 문 메시 설정
	SetupDoorMeshes();
}

void ADoor03::BeginPlay()
{
	Super::BeginPlay();
	
	// 초기 회전값 저장
	if (DoorMeshComponents.Num() >= 2)
	{
		InitialLocation = DoorMeshComponents[0]->GetRelativeLocation();
	}
}

void ADoor03::ApplyDoorAnimation(float Alpha)
{
	if (DoorMeshComponents.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door02: Not enough door mesh components!"));
		return;
	}

	// 타겟 회전값 계산
	CalculateTargetLocations();
	
	// 부드러운 보간을 사용하여 회전 적용
	const float SmoothedAlpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);
	
	const FVector CurrentLocation = FMath::Lerp(InitialLocation, TargetLocation, SmoothedAlpha);
	
	ApplyDoorLocation(DoorMeshComponents[0], CurrentLocation);

}

void ADoor03::SetupDoorMeshes()
{
	// 문 메시 에셋 로드
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RightMeshAsset(
		TEXT("/Game/JapaneseSchool/Meshes/WindowDoor/SM_Classroom_Door_01_R.SM_Classroom_Door_01_R")
	);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LeftMeshAsset(
	TEXT("/Game/JapaneseSchool/Meshes/WindowDoor/SM_Classroom_Door_01_L.SM_Classroom_Door_01_L")
);

	if (!RightMeshAsset.Succeeded() || !RightMeshAsset.Succeeded())
	{
		UE_LOG(LogTemp, Error, TEXT("Door02: Failed to load door mesh asset!"));
		return;
	}

	// 왼쪽 문 생성
	UStaticMeshComponent* LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	if (LeftDoor)
	{
		LeftDoor->SetStaticMesh(LeftMeshAsset.Object);
		LeftDoor->SetupAttachment(Root);
		LeftDoor->SetRelativeLocationAndRotation(FVector(0.f, 1.f, 0.f), FRotator::ZeroRotator);
		LeftDoor->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		DoorMeshComponents.Add(LeftDoor);
	}

	// 오른쪽 문 생성
	UStaticMeshComponent* RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	if (RightDoor)
	{
		RightDoor->SetStaticMesh(RightMeshAsset.Object);
		RightDoor->SetupAttachment(Root);
		RightDoor->SetRelativeLocationAndRotation(FVector(5.f, -6.f, 0.f), FRotator::ZeroRotator);
		RightDoor->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		DoorMeshComponents.Add(RightDoor);
	}

	// BoxCollision 설정 조정
	if (BoxCollision)
	{
		BoxCollision->SetRelativeLocationAndRotation(FVector(0.f, -10.f, 100.0f), FRotator::ZeroRotator);
		BoxCollision->SetRelativeScale3D(FVector(2.87262f, 3.832295f, 2.608312f));
	}
}

void ADoor03::CalculateTargetLocations()
{
	TargetLocation = InitialLocation + FVector(-OpenDistance, 0.0f, 0.0f);
}
