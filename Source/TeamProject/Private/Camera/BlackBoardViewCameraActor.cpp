#include "Camera/BlackBoardViewCameraActor.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"

void ABlackBoardViewCameraActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AMainMapGameMode * MainGameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			MainGameMode->SetBlackBoardViewCamera(this);
		}
	}
}
