#include "MainGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

UMainGameInstance::UMainGameInstance()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
	,FindSessionCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this,&ThisClass::OnFindSessionComplete))
	,JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnJoinSessionComplete))
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
	}
}

void UMainGameInstance::CreateGameSession()
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
	SessionSettings->Set(FName("MatchType"), FString("Mission"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//ù��° ���ڴ� ������ ����, �ι�° ���ڴ� ������ ���
	//-----------------------------------------------

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}

void UMainGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
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

void UMainGameInstance::JoinGameSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session Interface is invailed")));
		return;
	}

	//Sessionã���� ȣ���� Delegate���
	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegate);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void UMainGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface.IsValid())
		return;

	FString Address;
	if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Connect String : %s"), *Address));

		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (PlayerController)
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

void UMainGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (!OnlineSessionInterface.IsValid() || !bWasSuccessful)
	{
		if(GEngine)
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


		UE_LOG(LogTemp, Warning, TEXT("%s AFEFEFE"), *MatchType);

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
