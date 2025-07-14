#include "GameFrameWork/SplashTagGameInstance.h"

void USplashTagGameInstance::Init()
{
	Super::Init();

	if (GEngine)
		GEngine->OnNetworkFailure().AddUObject(this,&USplashTagGameInstance::NetworkFailure);
}

void USplashTagGameInstance::NetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,
	const FString& ErrorString)
{
	IOnlineSubsystem * Subsystem = IOnlineSubsystem::Get();

	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->DestroySession(NAME_GameSession);
		}
	}
}
