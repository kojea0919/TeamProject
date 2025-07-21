#include "GameFrameWork/MainMap/TaggerBlockBox/TaggerBlockBox.h"
#include "GameFrameWork/MainMap/MainMapGameState.h"
#include "Components/BoxComponent.h"

ATaggerBlockBox::ATaggerBlockBox()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BlockBox = CreateDefaultSubobject<UBoxComponent>("BlockBox");
	SetRootComponent(BlockBox);
}

void ATaggerBlockBox::SetBlock_Implementation(bool BlockOn)
{	
	if (BlockOn)
		BlockBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	else
		BlockBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ATaggerBlockBox::BeginPlay()
{
	Super::BeginPlay();
}
