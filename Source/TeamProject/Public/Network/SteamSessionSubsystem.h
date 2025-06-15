// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Logging/LogMacros.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SteamSessionSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API USteamSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void CreateGameSession();

	void JoinGameSession();

private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void OnCreateSessionComplete();
	
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//UFUNCTION()
	//void OnFindSessionsComplete(const TArray<FBlueprintSessionResult>& SessionResults);

private:
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

	//SessionList
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
