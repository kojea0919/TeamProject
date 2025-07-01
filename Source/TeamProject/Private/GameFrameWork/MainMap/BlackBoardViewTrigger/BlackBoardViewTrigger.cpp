#include "GameFrameWork/MainMap/BlackBoardViewTrigger/BlackBoardViewTrigger.h"

#include "Camera/BlackBoardViewCameraActor.h"
#include "Components/BoxComponent.h"
#include "Player/Character/RunnerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"

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
	if (HasAuthority())
	{
		ARunnerCharacter * Agent = Cast<ARunnerCharacter>(OtherActor);
		if (nullptr == Agent)
			return;
		
		if (Agent->IsLocallyControlled())
		{
			APlayerController * PlayerController = Cast<APlayerController>(Agent->GetController());
			AMainMapGameMode * MainMapGameMode = Cast<AMainMapGameMode>(GetWorld()->GetAuthGameMode());
			if (PlayerController && MainMapGameMode)
			{				
				PlayerController->SetViewTargetWithBlend(MainMapGameMode->GetBlackBoardViewCamera(),1.0f);
				PlayerController->SetInputMode(FInputModeGameAndUI());
				PlayerController->SetShowMouseCursor(true);
			}
		}
	}
}

void ABlackBoardViewTrigger::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		ARunnerCharacter * Agent = Cast<ARunnerCharacter>(OtherActor);
		if (nullptr == Agent)
			return;
		
		if (Agent->IsLocallyControlled())
		{
			APlayerController * PlayerController = Cast<APlayerController>(Agent->GetController());
			if (PlayerController)
			{				
				PlayerController->SetViewTargetWithBlend(Agent,1.0f);
				PlayerController->SetInputMode(FInputModeGameOnly());
				PlayerController->SetShowMouseCursor(false);
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
