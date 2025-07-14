#include "Map/Location/PrisonSpawnLocation.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"

APrisonSpawnLocation::APrisonSpawnLocation()
{
	PrimaryActorTick.bCanEverTick = false;

}

void APrisonSpawnLocation::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			GameMode->AddPrisonSpawnLocation(GetActorLocation());
		}
	}
	
}

