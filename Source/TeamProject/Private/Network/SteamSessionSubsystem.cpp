// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/SteamSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "IPAddress.h"
#include "SocketSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "AdvancedSessions.h"
#include "CreateSessionCallbackProxyAdvanced.h"
#include "FindSessionsCallbackProxyAdvanced.h"
#include "Kismet/GameplayStatics.h"

void USteamSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USteamSessionSubsystem::OnJoinSessionComplete);
}

void USteamSessionSubsystem::CreateGameSession()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is null"));
		return;
	}

	//ExtraSetting
	TArray<FSessionPropertyKeyPair> ExtraSettings;

	FSessionPropertyKeyPair MatchTypeSettings;
	MatchTypeSettings.Key = FName("MatchType");
	MatchTypeSettings.Data = FString("Mission");
	ExtraSettings.Add(MatchTypeSettings);
	
	//UCreateSessionCallbackProxyAdvanced * Proxy =  UCreateSessionCallbackProxyAdvanced::CreateAdvancedSession(GetWorld(),
	//	ExtraSettings, PlayerController,4,0,false, true,
	//	false,true,true,false,
	//	false,false,true,false,true);
	
	//Proxy->OnSuccess.AddDynamic(this, &USteamSessionSubsystem::OnCreateSessionComplete);

	
}

void USteamSessionSubsystem::OnCreateSessionComplete()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Red, FString::Printf(TEXT("Complete Create")));
	}
	
	if (UWorld* World = GetWorld())
		World->ServerTravel(FString("/Game/_GamePlay/Map/MainMap?listen"));
}

void USteamSessionSubsystem::JoinGameSession()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is null"));
		return;
	}
	
	TArray<FSessionsSearchSetting> Filters;

	FSessionsSearchSetting Filter;
	Filter.PropertyKeyPair.Key = FName("MatchType");
	Filter.PropertyKeyPair.Data = FString("Mission");
	Filters.Add(Filter);

	
	//UFindSessionsCallbackProxyAdvanced * Proxy = UFindSessionsCallbackProxyAdvanced::FindSessionsAdvanced(
	//	GetWorld(), PlayerController, 100, false, EBPServerPresenceSearchType::AllServers, Filters);
	
	//Proxy->OnSuccess.AddDynamic(this, &USteamSessionSubsystem::OnFindSessionsComplete);
}


//void UMainGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
//{
//	if (!OnlineSessionInterface.IsValid())
//		return;
//
//	FString Address;
//	if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
//	{
//		if (GEngine)
//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Connect String : %s"), *Address));
//
//
//
//		APlayerController* PlayerController = GetFirstLocalPlayerController();
//		if (PlayerController)
//			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
//	}
//}

void USteamSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
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
		// NAT Loopback ��ȸ �ڵ� ��

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Connect String : %s"), *Address));

		APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (PlayerController)
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}


//void USteamSessionSubsystem::OnFindSessionsComplete(const TArray<FBlueprintSessionResult>& SessionResults)
//{
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(
//			-1, 15.f, FColor::Red, FString::Printf(TEXT("%d"), SessionSearch->SearchResults.Num()));
//	}
//
//	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
//	IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
//
//	for (auto& Result : SessionSearch->SearchResults)
//	{
//		FString Id = Result.GetSessionIdStr();
//		FString User = Result.Session.OwningUserName;
//
//		FString MatchType;
//		Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);
//
//		if (MatchType == FString("Mission"))
//		{
//			if (GEngine)
//				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Joining Match Type : %s"), *MatchType));
//
//			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
//
//			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
//			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
//		}
//	}
//}

