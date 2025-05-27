// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Logging/LogMacros.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMainGameInstance();

public:
	void CreateGameSession();
	void JoinGameSession();

private:
	//Session���� ������ Delegate �Լ�
	//---------------------------------------
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;

	//CreateSession���� �� ȣ�� �Լ�
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	//---------------------------------------


	//Session���� ������ Delegate �Լ�
	//---------------------------------------
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	//---------------------------------------

	//Sessionã�� ������ Delegate �Լ�
	//---------------------------------------
	void OnFindSessionComplete(bool bWasSuccessful);

	FOnFindSessionsCompleteDelegate FindSessionCompleteDelegate;
	//---------------------------------------

	IOnlineSessionPtr OnlineSessionInterface;

private:
	//SessionList
	TSharedPtr<FOnlineSessionSearch> SessionSearch; 
};
