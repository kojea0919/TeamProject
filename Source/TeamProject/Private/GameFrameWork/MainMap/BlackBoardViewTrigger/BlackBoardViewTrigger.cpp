#include "GameFrameWork/MainMap/BlackBoardViewTrigger/BlackBoardViewTrigger.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "GameFrameWork/MainMap/MainMapGameState.h"
#include "Camera/BlackBoardViewCameraActor.h"
#include "Components/BoxComponent.h"
#include "Player/Character/RunnerCharacter.h"


ABlackBoardViewTrigger::ABlackBoardViewTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);

	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap); // 캐릭터만 감지
}

void ABlackBoardViewTrigger::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainMapGameState * GameState = GetWorld()->GetGameState<AMainMapGameState>();
	if (nullptr == GameState || GameState->GetCurrentGameState() == EGameState::Playing)
		return;	
	
	if (HasAuthority())
	{
		ARunnerCharacter * Runner = Cast<ARunnerCharacter>(OtherActor);
		if (nullptr == Runner)
			return;
		
		if (Runner->IsLocallyControlled())
		{
			AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(Runner->GetController());
			AMainMapGameMode * MainMapGameMode = Cast<AMainMapGameMode>(GetWorld()->GetAuthGameMode());
			if (PlayerController && MainMapGameMode)
			{				
				PlayerController->SetViewTargetWithBlend(MainMapGameMode->GetBlackBoardViewCamera(),1.0f);
				PlayerController->SetInputMode(FInputModeGameAndUI());
				PlayerController->SetShowMouseCursor(true);
				PlayerController->SetVisibleBlackBoard(true);
			}
		}
	}
}

void ABlackBoardViewTrigger::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainMapGameState * GameState = GetWorld()->GetGameState<AMainMapGameState>();
	if (nullptr == GameState || GameState->GetCurrentGameState() == EGameState::Playing)
		return;
	
	if (HasAuthority())
	{
		ARunnerCharacter * Runner = Cast<ARunnerCharacter>(OtherActor);
		if (nullptr == Runner)
			return;
		
		if (Runner->IsLocallyControlled())
		{
			AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(Runner->GetController());
			if (PlayerController)
			{				
				PlayerController->SetViewTargetWithBlend(Runner,1.0f);
				PlayerController->SetInputMode(FInputModeGameOnly());
				PlayerController->SetShowMouseCursor(false);
				PlayerController->SetVisibleBlackBoard(false);
			}
		}
	}
}

void ABlackBoardViewTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABlackBoardViewTrigger::BeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ABlackBoardViewTrigger::EndOverlap);
	
}
