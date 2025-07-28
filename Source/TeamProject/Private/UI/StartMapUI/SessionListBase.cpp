// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartMapUI/SessionListBase.h"

#include "OnlineSubsystemUtils.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "UI/StartMapUI/SessionList.h"

void USessionListBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Exit)
		Btn_Exit->OnClicked.AddDynamic(this,&USessionListBase::ClickExit);
}

void USessionListBase::AddSessionList(FBlueprintSessionResult SessionResult)
{
	if (VB_SessionListbox && SessionListClass)
	{
		FOnlineSessionSearchResult SearchResult = SessionResult.OnlineResult;
		TSharedPtr<const FUniqueNetId> OwnerId = SearchResult.Session.OwningUserId;
		IOnlineSubsystem * OnlineSub = Online::GetSubsystem(GetWorld(),STEAM_SUBSYSTEM);

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
		// else
		// 	return;

		int32 PlayerNum;
		SearchResult.Session.SessionSettings.Get(FName("CurrentPlayerCount"),PlayerNum);

		USessionList * NewSession = CreateWidget<USessionList>(GetWorld(),SessionListClass);
		if (nullptr == NewSession)
			return;
		
		if (OwnerId.IsValid())
		{
			NewSession->SetHostName(HostNickName);
		}
		NewSession->SetSessionPlayerNumber(PlayerNum);
		NewSession->SetSessionResult(SessionResult);
		
		VB_SessionListbox->AddChildToVerticalBox(NewSession);
	}
}

void USessionListBase::ClearSessionList()
{
	if (VB_SessionListbox)
		VB_SessionListbox->ClearChildren();
}

void USessionListBase::SetVisibleSessionLoadImage(bool bVisible)
{
	if (nullptr == WB_LoadingSession)
		return;
	
	if (bVisible)
		WB_LoadingSession->SetVisibility(ESlateVisibility::Visible);
	else
		WB_LoadingSession->SetVisibility(ESlateVisibility::Hidden);
}

void USessionListBase::ClickExit()
{
	OnFrameExitButtonClicked.Execute();
}
