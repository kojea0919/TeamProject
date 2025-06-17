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

	// Overlap 이벤트 바인딩 (서버/클라이언트 모두)
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseDoor::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseDoor::OnOverlapEnd);

	// 초기 애니메이션 상태 적용
	ApplyDoorAnimation(CurrentAlpha);
}

void ABaseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 문 애니메이션 업데이트 (서버에서만 로직 처리)
	if (HasAuthority() && (CurrentDoorState == EDoorState::Opening || CurrentDoorState == EDoorState::Closing))
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
	// 서버에서만 Alpha 값 계산
	if (!HasAuthority())
	{
		return;
	}

	float NewAlpha = CurrentAlpha;
	
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
		SetDoorAlpha(NewAlpha);
	}
}

void ABaseDoor::SetDoorAlpha(float NewAlpha)
{
	// 서버에서만 호출
	if (!HasAuthority())
	{
		return;
	}

	CurrentAlpha = NewAlpha;
	
	// 서버에서도 즉시 애니메이션 적용 (OnRep은 서버에서 호출되지 않으므로)
	ApplyDoorAnimation(CurrentAlpha);
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
		
		// 서버에서도 즉시 OnRep 로직 실행 (OnRep은 서버에서 호출되지 않으므로)
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
	if (!IsValidOverlappingActor(OtherActor))
	{
		return;
	}

	// 서버에서 직접 처리
	if (HasAuthority())
	{
		// 첫 번째 액터가 들어올 때만 방향 계산
		if (OverlappingActorCount == 0 && CurrentDoorState == EDoorState::Closed)
		{
			bOpenTowardsFront = FindDoorOpenDirection(OtherActor);
		}

		OverlappingActorCount++;
		
		// 문 열기
		OpenDoor();
		
		UE_LOG(LogTemp, Warning, TEXT("Server: Actor entered - Count: %d, Direction: %s"), 
			OverlappingActorCount, bOpenTowardsFront ? TEXT("Front") : TEXT("Back"));
			
		// 뷰포트에 표시
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, 
				FString::Printf(TEXT("Server: Door Overlap Begin - Count: %d"), OverlappingActorCount));
		}
	}
	else
	{
		// 클라이언트에서는 서버에 RPC 요청
		ServerRequestDoorOpen(OtherActor);
		
		UE_LOG(LogTemp, Warning, TEXT("Client: Requesting door open via RPC"));
		
		// 뷰포트에 표시
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, 
				TEXT("Client: Requesting Door Open"));
		}
	}
}

void ABaseDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValidOverlappingActor(OtherActor))
	{
		return;
	}

	// 서버에서 직접 처리
	if (HasAuthority())
	{
		OverlappingActorCount = FMath::Max(0, OverlappingActorCount - 1);
		
		UE_LOG(LogTemp, Warning, TEXT("Server: Actor exited - Count: %d"), OverlappingActorCount);
		
		// 뷰포트에 표시
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, 
				FString::Printf(TEXT("Server: Door Overlap End - Count: %d"), OverlappingActorCount));
		}
		
		// 마지막 액터가 나갔을 때 문 닫기
		if (OverlappingActorCount == 0)
		{
			CloseDoor();
		}
	}
	else
	{
		// 클라이언트에서는 서버에 RPC 요청
		ServerRequestDoorClose();
		
		UE_LOG(LogTemp, Warning, TEXT("Client: Requesting door close via RPC"));
		
		// 뷰포트에 표시
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, 
				TEXT("Client: Requesting Door Close"));
		}
	}
}

void ABaseDoor::ApplyDoorRotation(UStaticMeshComponent* Door, const FRotator& Rotation)
{
	if (Door)
	{
		Door->SetRelativeRotation(Rotation);
	}
}

void ABaseDoor::ApplyDoorLocation(UStaticMeshComponent* Door, const FVector& Location)
{
	if (Door)
	{
		Door->SetRelativeLocation(Location);
	}
}

bool ABaseDoor::IsValidOverlappingActor(AActor* Actor) const
{
	return Actor != nullptr;
}

// OnRep 함수들 (클라이언트에서만 호출됨)
void ABaseDoor::OnRep_DoorState()
{
	// 상태 변경시 클라이언트에서 실행할 로직
	UE_LOG(LogTemp, Log, TEXT("Client: Door state replicated to: %d"), (int32)CurrentDoorState);
	
	// 뷰포트에 메시지 표시
	if (GEngine)
	{
		FString StateString;
		switch (CurrentDoorState)
		{
			case EDoorState::Closed: StateString = TEXT("Closed"); break;
			case EDoorState::Opening: StateString = TEXT("Opening"); break;
			case EDoorState::Open: StateString = TEXT("Open"); break;
			case EDoorState::Closing: StateString = TEXT("Closing"); break;
		}
		
		GEngine->AddOnScreenDebugMessage(
			-1, // 고유 키 (이 액터의 ID 사용)
			3.0f, // 3초간 표시
			FColor::Cyan, // 시안 색상
			FString::Printf(TEXT("Client OnRep: Door State: %s"), *StateString)
		);
	}
	
	// 필요시 사운드나 파티클 이펙트 등을 여기서 처리
}

void ABaseDoor::OnRep_DoorAlpha()
{
	// 클라이언트에서 Alpha 값이 복제되면 즉시 애니메이션 적용
	ApplyDoorAnimation(CurrentAlpha);
	
	UE_LOG(LogTemp, Log, TEXT("Client: Door alpha replicated to: %f"), CurrentAlpha);
	
	// 뷰포트에 표시
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, // 고유 키 (상태와 다르게)
			2.0f, 
			FColor::Magenta, 
			FString::Printf(TEXT("Client OnRep: Door Alpha: %.2f"), CurrentAlpha)
		);
	}
}

// 서버 RPC 구현
void ABaseDoor::ServerRequestDoorOpen_Implementation(AActor* RequestingActor)
{
	// 서버에서 요청 처리 - 실제로 Overlap 중인지 검증
	if (!IsValidOverlappingActor(RequestingActor))
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple,TEXT("Server Request Called"));
	
	// 서버에서 실제 Overlap 상태 확인 (보안 검증)
	TArray<AActor*> OverlappingActors;
	BoxCollision->GetOverlappingActors(OverlappingActors);
	
	bool bActorIsOverlapping = OverlappingActors.Contains(RequestingActor);
	if (!bActorIsOverlapping)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server: RPC request denied - Actor not actually overlapping"));
		return;
	}
	
	// 첫 번째 액터가 들어올 때만 방향 계산
	if (OverlappingActorCount == 0  && CurrentDoorState == EDoorState::Closed)
	{
		bOpenTowardsFront = FindDoorOpenDirection(RequestingActor);
	}

	OverlappingActorCount++;
	
	// 문 열기
	OpenDoor();
	
	UE_LOG(LogTemp, Warning, TEXT("Server: RPC Door Open - Count: %d, Direction: %s"), 
		OverlappingActorCount, bOpenTowardsFront ? TEXT("Front") : TEXT("Back"));
		
	// 뷰포트에 표시
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, 
			FString::Printf(TEXT("Server: RPC Door Open - Count: %d"), OverlappingActorCount));
	}
}

void ABaseDoor::ServerRequestDoorClose_Implementation()
{
	OverlappingActorCount = FMath::Max(0, OverlappingActorCount - 1);
	
	UE_LOG(LogTemp, Warning, TEXT("Server: RPC Door Close - Count: %d"), OverlappingActorCount);
	
	// 뷰포트에 표시
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
			FString::Printf(TEXT("Server: RPC Door Close - Count: %d"), OverlappingActorCount));
	}
	
	// 마지막 액터가 나갔을 때 문 닫기
	if (OverlappingActorCount == 0)
	{
		CloseDoor();
	}
}