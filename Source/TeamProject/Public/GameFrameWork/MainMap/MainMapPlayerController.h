// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/SmartPhone/SmartPhoneEnumType.h"
#include "MainMapPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AMainMapPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	void UpdateRemainTime(int Second);

	UFUNCTION(BlueprintCallable)
	void SetActiveMic(bool Active);
	
	UFUNCTION(BlueprintCallable)
	void SetTalkingMic();

	UFUNCTION(Server, Reliable)
	void SendChatMessageServer(const FText & Text, EChattingRoomType RoomType);
	void SendChatMessageServer_Implementation(const FText & Text, EChattingRoomType RoomType);
	
	void SendAllChatMessage(const FText & Text, const FString & SendPlayerNickName);
	void SendTeamChatMessage(const FText & Text, const FString & SendPlayerNickName);

	UFUNCTION(Client, Reliable)
	void RecvSelfAllChatMessage(const FText & Text);
	void RecvSelfAllChatMessage_Implementation(const FText & Text);

	UFUNCTION(Client, Reliable)
	void RecvOtherAllChatMessage(const FText & Text, const FString & SendPlayerNickName);
	void RecvOtherAllChatMessage_Implementation(const FText & Text, const FString & SendPlayerNickName);

	UFUNCTION(Client, Reliable)
	void RecvSelfTeamChatMessage(const FText & Text);
	void RecvSelfTeamChatMessage_Implementation(const FText & Text);

	UFUNCTION(Client, Reliable)
	void RecvOtherTeamChatMessage(const FText & Text, const FString & SendPlayerNickName);
	void RecvOtherTeamChatMessage_Implementation(const FText & Text, const FString & SendPlayerNickName);
	
	
private:
	void InitInputMode();
	void InitHUD();
	
private:
	//Main HUD
	//----------------------------------
	UPROPERTY(EditAnywhere,Category=HUD, meta = (AllowPrivateAccess=true))
	TSubclassOf<class UPlayerMainHUD> PlayerMainHUDWidgetClass;

	UPROPERTY()
	class UPlayerMainHUD* PlayerMainHUD;
	//----------------------------------
};
