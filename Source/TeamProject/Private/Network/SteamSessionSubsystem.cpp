// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/SteamSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "IPAddress.h"
#include "SocketSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

void USteamSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// Initialize the Online Session Interface
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
	}

	CreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USteamSessionSubsystem::OnCreateSessionComplete);
	FindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USteamSessionSubsystem::OnFindSessionsComplete);
	JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USteamSessionSubsystem::OnJoinSessionComplete);
}

void USteamSessionSubsystem::CreateGameSession()
{
	if (!OnlineSessionInterface.IsValid())
		return;

	//�ش� �̸��� ���� ������ �̹� �����ϸ� �ı�
	auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}

	//CreateSession�� ������ ȣ���� Delegate�� DelegateList�� �߰�
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	//Create Session Setting Object
	//-----------------------------------------------
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = false;			//�� ������ ���� ���ϴ� ����
	SessionSettings->NumPublicConnections = 4;		//������ ���� ��
	SessionSettings->bAllowJoinInProgress = true;	//���� ���� �������̸� �ٸ������� ���� ��������
	SessionSettings->bAllowJoinViaPresence = true;	//ģ��	����Ʈ���� Join?
	SessionSettings->bShouldAdvertise = true;		//���� ����
	SessionSettings->bUseLobbiesIfAvailable = true;	//�κ� �ý���
	SessionSettings->bUsesPresence = true;
	SessionSettings->Set(FName("MatchType"), FString("Mission"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//ù��° ���ڴ� ������ ����, �ι�° ���ڴ� ������ ���
	//-----------------------------------------------

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}

void USteamSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Red, FString::Printf(TEXT("Complete Create")));
	}

	//CreateSesison ������ Lobby�� ListemServer�� ����
	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if (World)
			World->ServerTravel(FString("/Game/_GamePlay/Map/MainMap?listen"));
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to Create Session")));
		}
	}
}

void USteamSessionSubsystem::JoinGameSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session Interface is invailed")));
		return;
	}

	//Sessionã���� ȣ���� Delegate���
	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionSearch->bIsLanQuery = false;

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
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
	if (!OnlineSessionInterface.IsValid())
		return;

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


void USteamSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid() || !bWasSuccessful)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Failed")));
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Red, FString::Printf(TEXT("%d"), SessionSearch->SearchResults.Num()));
	}


	for (auto Result : SessionSearch->SearchResults)
	{
		FString Id = Result.GetSessionIdStr();
		FString User = Result.Session.OwningUserName;

		FString MatchType;
		Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);

		if (MatchType == FString("Mission"))
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Joining Match Type : %s"), *MatchType));

			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
		}
	}
}

