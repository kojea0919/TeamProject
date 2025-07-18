#include "GameFrameWork/MainMap/PrisonCollisionBox/PrisonCollisionBox.h"
#include "Components/BoxComponent.h"
#include "GameFrameWork/MainMap/MainMapGameState.h"
#include "Player/Character/BaseCharacter.h"
#include "GameTag/STGamePlayTags.h"

APrisonCollisionBox::APrisonCollisionBox()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	SetRootComponent(BoxComponent);
}

int APrisonCollisionBox::Check()
{	
	TArray<FHitResult> HitResults;
	FVector Location = GetActorLocation();
	FVector BoxSize;
	if (BoxComponent)
		BoxSize = BoxComponent->GetScaledBoxExtent();
	else
		BoxSize = FVector(1,1,1);
	
	FCollisionShape BoxShape = FCollisionShape::MakeBox(BoxSize);

	bool Hit = GetWorld()->SweepMultiByChannel(HitResults ,Location,Location,FQuat::Identity,ECC_Pawn,BoxShape);
	if (Hit)
	{
		int32 Num = HitResults.Num();
		int RunnerCount = 0;
		for(int32 Idx = 0; Idx < Num; ++Idx)
		{
			ABaseCharacter * CurActor = Cast<ABaseCharacter>(HitResults[Idx].GetActor());
			if (CurActor && CurActor->GetBaseCharacterTag() == STGamePlayTags::Player_Runner )
			{
				++RunnerCount;
			}
		}
		return RunnerCount;
	}
	else
	{
		return 0;
	}
}

void APrisonCollisionBox::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AMainMapGameState * GameState = GetWorld()->GetGameState<AMainMapGameState>();
		if (GameState)
			GameState->RegisterPrisonCollisionBox(this);
	}
}

