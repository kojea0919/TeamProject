// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartMapUI/SessionListBase.h"
#include "Components/VerticalBox.h"
#include "UI/StartMapUI/SessionList.h"

void USessionListBase::AddSessionList(FBlueprintSessionResult SessionResult)
{
	if (VB_SessionListbox && SessionListClass)
	{
		USessionList * NewSession = CreateWidget<USessionList>(GetWorld(),SessionListClass);
	
		FOnlineSessionSearchResult SearchResult = SessionResult.OnlineResult;
		TSharedPtr<const FUniqueNetId> OwnerId = SearchResult.Session.OwningUserId;
		if (OwnerId.IsValid())
		{
			FString SteamIdStr = OwnerId->ToString();
			NewSession->SetHostName(SteamIdStr);
		}
		
		NewSession->SetSessionPlayerNumber(SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections);
		
		VB_SessionListbox->AddChildToVerticalBox(NewSession);
	}
}
