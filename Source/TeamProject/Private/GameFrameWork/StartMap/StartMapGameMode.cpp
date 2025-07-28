#include "GameFrameWork/StartMap/StartMapGameMode.h"
#include "GameFrameWork/StartMap/StartMapPlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "IPAddress.h"
#include "OnlineSubsystemUtils.h"
#include "SocketSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

FString AStartMapGameMode::GetAddress()
{
	IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld(),STEAM_SUBSYSTEM);
	IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

	FString Address;

	if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		FString LocalIP;
		bool bGotLocalIP = false;

		bool bCanBindAll;
		TSharedRef<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
		if (LocalAddr->IsValid())
		{
			LocalIP = LocalAddr->ToString(false);
			bGotLocalIP = true;
		}

		FString HostIP, Port;
		if (Address.Split(":", &HostIP, &Port))
		{
			if (HostIP == IOnlineSubsystem::Get()->GetIdentityInterface()->GetUniquePlayerId(0)->ToString())
			{
				if (bGotLocalIP)
				{
					Address = FString::Printf(TEXT("%s:%s"), *LocalIP, *Port);
				}
			}
		}
	}

#if WITH_EDITOR || UE_BUILD_DEBUG
	if(!Address.IsEmpty())
		GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Red, Address);	
#endif

	return Address;
}

void AStartMapGameMode::ClientTravel(const FString & Address)
{
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PlayerController)
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void AStartMapGameMode::AddToSessionList(FBlueprintSessionResult SessionResult)
{
	if (AStartMapPlayerController* PlayerController =
		Cast<AStartMapPlayerController>(GetGameInstance()->GetFirstLocalPlayerController()))
	{
		PlayerController->AddToSessionList(SessionResult);
	}
}

void AStartMapGameMode::PlayBGM()
{
	if (StartMapBGM)
	{
		UGameplayStatics::PlaySound2D(GetWorld(),StartMapBGM);
	}
}

void AStartMapGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayBGM();
}
