#include "Map/Mesh/Door/BaseDoor.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

ABaseDoor::ABaseDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root Component 설정
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Box Collision 설정
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Root);
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollision->SetGenerateOverlapEvents(true);
	
	// 기본값 설정
	CurrentDoorState = EDoorState::Closed;
	CurrentAlpha = 0.0f;
	OverlappingActorCount = 0;
}

void ABaseDoor::BeginPlay()
{
	Super::BeginPlay();

	// Overlap 이벤트 바인딩
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseDoor::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseDoor::OnOverlapEnd);
}

void ABaseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 문 애니메이션 업데이트
	if (CurrentDoorState == EDoorState::Opening || CurrentDoorState == EDoorState::Closing)
	{
		UpdateDoorAnimation(DeltaTime);
	}
}

bool ABaseDoor::FindDoorOpenDirection(const AActor* PlayerActor) const
{
	if (!PlayerActor)
	{
		return false;
	}

	const FVector DoorLocation = GetActorLocation();
	const FVector PlayerLocation = PlayerActor->GetActorLocation();
	const FVector DirToPlayer = PlayerLocation - DoorLocation;

	// 로컬 좌표계로 변환하여 플레이어가 문의 앞쪽에 있는지 확인
	const FVector LocalDirection = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), DirToPlayer);
	
	return LocalDirection.Y > 0.0f;
}

void ABaseDoor::SetDoorState(EDoorState NewState)
{
	if (CurrentDoorState != NewState)
	{
		CurrentDoorState = NewState;
		
		// 상태 변경 로그
		UE_LOG(LogTemp, Log, TEXT("Door state changed to: %d"), (int32)NewState);
	}
}

void ABaseDoor::OpenDoor()
{
	if (CurrentDoorState == EDoorState::Closed || CurrentDoorState == EDoorState::Closing)
	{
		SetDoorState(EDoorState::Opening);
		UE_LOG(LogTemp, Log, TEXT("Door opening - Direction: %s"), bOpenTowardsFront ? TEXT("Front") : TEXT("Back"));
	}
}

void ABaseDoor::CloseDoor()
{
	if (CurrentDoorState == EDoorState::Open || CurrentDoorState == EDoorState::Opening)
	{
		SetDoorState(EDoorState::Closing);
		UE_LOG(LogTemp, Log, TEXT("Door closing"));
	}
}

void ABaseDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlappingActor(OtherActor))
	{
		return;
	}

	// 첫 번째 액터가 들어올 때만 방향 계산
	if (OverlappingActorCount == 0)
	{
		bOpenTowardsFront = FindDoorOpenDirection(OtherActor);
	}

	OverlappingActorCount++;
	
	// 문 열기
	OpenDoor();
	
	UE_LOG(LogTemp, Warning, TEXT("Actor entered - Count: %d, Direction: %s"), 
		OverlappingActorCount, bOpenTowardsFront ? TEXT("Front") : TEXT("Back"));
}

void ABaseDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValidOverlappingActor(OtherActor))
	{
		return;
	}

	OverlappingActorCount = FMath::Max(0, OverlappingActorCount - 1);
	
	UE_LOG(LogTemp, Warning, TEXT("Actor exited - Count: %d"), OverlappingActorCount);
	
	// 마지막 액터가 나갔을 때 문 닫기
	if (OverlappingActorCount == 0)
	{
		CloseDoor();
	}
}

void ABaseDoor::ApplyDoorRotation(UStaticMeshComponent* Door, const FRotator& Rotation)
{
	if (Door)
	{
		Door->SetRelativeRotation(Rotation);
	}
}

bool ABaseDoor::IsValidOverlappingActor(AActor* Actor) const
{
	// 기본적으로 Character 클래스만 허용 (필요에 따라 수정 가능)
	return Actor != nullptr;
	//return Actor && Actor != this && Actor->IsA<ACharacter>();
}