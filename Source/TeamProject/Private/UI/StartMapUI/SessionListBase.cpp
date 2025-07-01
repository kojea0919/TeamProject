// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartMapUI/SessionListBase.h"
#include "Components/VerticalBox.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "UI/StartMapUI/SessionList.h"

void USessionListBase::AddSessionList(FBlueprintSessionResult SessionResult)
{
	if (VB_SessionListbox && SessionListClass)
	{
		FOnlineSessionSearchResult SearchResult = SessionResult.OnlineResult;
		TSharedPtr<const FUniqueNetId> OwnerId = SearchResult.Session.OwningUserId;
		
		IOnlineSubsystem * OnlineSub = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
		FString HostNickName;
		if (OnlineSub)
		{
			IOnlineIdentityPtr IdentityInterface = OnlineSub->GetIdentityInterface();
			if (IdentityInterface.IsValid())
			{
				HostNickName = IdentityInterface->GetPlayerNickname(*OwnerId);
			}
			else
				return;
		}
		else
			return;
		


		
		USessionList * NewSession = CreateWidget<USessionList>(GetWorld(),SessionListClass);
		if (OwnerId.IsValid())
		{
			NewSession->SetHostName(HostNickName);
		}
		
		NewSession->SetSessionPlayerNumber(SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections);
		
		VB_SessionListbox->AddChildToVerticalBox(NewSession);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Frame NULL");
	}
}
