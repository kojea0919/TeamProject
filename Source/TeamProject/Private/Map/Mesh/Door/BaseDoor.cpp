#include "Map/Mesh/Door/BaseDoor.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

ABaseDoor::ABaseDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	// 네트워크 복제 활성화
	bReplicates = true;
	// 문 타입에 따라 위치 복제를 다르게 설정할 수 있도록 기본값만 설정
	// 슬라이딩 문의 경우 자식 클래스에서 SetReplicateMovement(true) 호출 가능

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
	PreviousAlpha = 0.0f;
	OverlappingActorCount = 0;
}

void ABaseDoor::BeginPlay()
{
	Super::BeginPlay();

	// Overlap 이벤트 바인딩 (서버에서만)
	if (HasAuthority())
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseDoor::OnOverlapBegin);
		BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseDoor::OnOverlapEnd);
	}

	// 초기 애니메이션 상태 적용
	ApplyDoorAnimation(CurrentAlpha);
}

void ABaseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 문 애니메이션 업데이트 (서버에서만 로직 처리, 클라이언트는 시각적 업데이트만)
	if (CurrentDoorState == EDoorState::Opening || CurrentDoorState == EDoorState::Closing)
	{
		UpdateDoorAnimation(DeltaTime);
	}
}

void ABaseDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 프로퍼티들 등록
	DOREPLIFETIME(ABaseDoor, CurrentDoorState);
	DOREPLIFETIME(ABaseDoor, CurrentAlpha);
	DOREPLIFETIME(ABaseDoor, bOpenTowardsFront);
}

void ABaseDoor::UpdateDoorAnimation(float DeltaTime)
{
	float NewAlpha = CurrentAlpha;
	
	// 서버에서만 Alpha 값 계산
	if (HasAuthority())
	{
		// 상태에 따른 알파값 업데이트
		switch (CurrentDoorState)
		{
			case EDoorState::Opening:
			{
				NewAlpha = FMath::Clamp(CurrentAlpha + DoorSpeed * DeltaTime, 0.0f, 1.0f);
				
				// 완전히 열렸을 때
				if (NewAlpha >= 1.0f)
				{
					SetDoorState(EDoorState::Open);
				}
				break;
			}
			
			case EDoorState::Closing:
			{
				NewAlpha = FMath::Clamp(CurrentAlpha - DoorSpeed * DeltaTime, 0.0f, 1.0f);
				
				// 완전히 닫혔을 때
				if (NewAlpha <= 0.0f)
				{
					SetDoorState(EDoorState::Closed);
				}
				break;
			}
			
			default:
				return;
		}
		
		// Alpha 값이 변경되었을 때만 업데이트
		if (FMath::Abs(NewAlpha - CurrentAlpha) > KINDA_SMALL_NUMBER)
		{
			CurrentAlpha = NewAlpha;
		}
	}
	
	// Alpha 값 변화가 있으면 애니메이션 적용 (서버/클라이언트 모두)
	if (FMath::Abs(CurrentAlpha - PreviousAlpha) > KINDA_SMALL_NUMBER)
	{
		ApplyDoorAnimation(CurrentAlpha);
		PreviousAlpha = CurrentAlpha;
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
	// 서버에서만 상태 변경 허용
	if (!HasAuthority())
	{
		return;
	}

	if (CurrentDoorState != NewState)
	{
		CurrentDoorState = NewState;
		
		// 상태 변경 로그
		UE_LOG(LogTemp, Log, TEXT("Door state changed to: %d"), (int32)NewState);
		
		// 클라이언트들에게 상태 변경 알림
		OnRep_DoorState();
	}
}

void ABaseDoor::OpenDoor()
{
	// 서버에서만 실행
	if (!HasAuthority())
	{
		return;
	}

	if (CurrentDoorState == EDoorState::Closed || CurrentDoorState == EDoorState::Closing)
	{
		SetDoorState(EDoorState::Opening);
		UE_LOG(LogTemp, Log, TEXT("Door opening - Direction: %s"), bOpenTowardsFront ? TEXT("Front") : TEXT("Back"));
	}
}

void ABaseDoor::CloseDoor()
{
	// 서버에서만 실행
	if (!HasAuthority())
	{
		return;
	}

	if (CurrentDoorState == EDoorState::Open || CurrentDoorState == EDoorState::Opening)
	{
		SetDoorState(EDoorState::Closing);
		UE_LOG(LogTemp, Log, TEXT("Door closing"));
	}
}

void ABaseDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 서버에서만 처리
	if (!HasAuthority())
	{
		return;
	}

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
	// 서버에서만 처리
	if (!HasAuthority())
	{
		return;
	}

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

void ABaseDoor::OnRep_DoorState()
{
	// 상태 변경시 클라이언트에서 실행할 로직
	UE_LOG(LogTemp, Log, TEXT("Client: Door state replicated to: %d"), (int32)CurrentDoorState);
	
	// 필요시 사운드나 파티클 이펙트 등을 여기서 처리
}

void ABaseDoor::OnRep_DoorAlpha()
{
	// Alpha 값이 복제되면 즉시 애니메이션 적용
	ApplyDoorAnimation(CurrentAlpha);
	PreviousAlpha = CurrentAlpha;
}

void ABaseDoor::ServerSetDoorState_Implementation(EDoorState NewState, bool bNewOpenDirection)
{
	// 서버에서 상태 변경 요청 처리
	bOpenTowardsFront = bNewOpenDirection;
	SetDoorState(NewState);
}