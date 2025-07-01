// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FindSessionsCallbackProxy.h"
#include "StartMapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AStartMapGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FString GetAddress();

	UFUNCTION(BlueprintCallable)
	void ClientTravel(const FString& Address);

	UFUNCTION(BlueprintCallable)
	void AddToSessionList(FBlueprintSessionResult SessionResult);


private:
	
	
};
