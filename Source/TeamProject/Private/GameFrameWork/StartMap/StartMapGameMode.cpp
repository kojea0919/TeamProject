#include "GameFrameWork/StartMap/StartMapGameMode.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "IPAddress.h"
#include "SocketSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"

FString AStartMapGameMode::GetAddress()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

	FString Address;
	if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		// NAT Loopback ��ȸ �ڵ� ����
		FString LocalIP;
		bool bGotLocalIP = false;

		// Get local IP address
		bool bCanBindAll;
		TSharedRef<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
		if (LocalAddr->IsValid())
		{
			LocalIP = LocalAddr->ToString(false); // false = exclude port
			bGotLocalIP = true;
		}

		// Extract host IP from resolved address (e.g., 192.168.0.10:7777)
		FString HostIP, Port;
		if (Address.Split(":", &HostIP, &Port))
		{
			// NAT loopback ��ȸ: �ڱ� �ڽſ��� ���� �� �ܺ� IP ��� ���� IP ���
			if (HostIP == IOnlineSubsystem::Get()->GetIdentityInterface()->GetUniquePlayerId(0)->ToString())
			{
				if (bGotLocalIP)
				{
					Address = FString::Printf(TEXT("%s:%s"), *LocalIP, *Port);
				}
			}
		}
	}

	return Address;
}

void AStartMapGameMode::ClientTravel(const FString & Address)
{
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PlayerController)
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
