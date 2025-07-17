// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"
#include "UI/SmartPhone/SmartPhoneEnumType.h"
#include "Player/Character/BaseType/BaseStructType.h"
#include "UI/MainHUD/PlayerItemSlot.h"
#include "MainMapPlayerController.generated.h"



class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;
class USTInputConfig;

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
	
	UFUNCTION(Client, Reliable)
	void ShowRole(bool IsTagger);
	void ShowRole_Implementation(bool IsTagger);

	// 메인 위젯 접근
	UFUNCTION(BlueprintCallable)
	class UPlayerMainHUD* GetPlayerMainHUD() const {return PlayerMainHUD;}

	UFUNCTION(Client, Reliable)
	void SetJobText(bool IsTagger);
	void SetJobText_Implementation(bool IsTagger);
	
	void SetPlayerNickName(FString NickName) { PlayerNickName = NickName; }
	const FString & GetNickName() const { return PlayerNickName; }

	void SetVisibleBlackBoard(bool Visible);
	
private:
	void InitInputMode();
	void InitWidget();
	
private:
	//Main HUD
	//----------------------------------
	UPROPERTY(EditAnywhere,Category = Widget, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UPlayerMainHUD> PlayerMainHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPlayerMainHUD> PlayerMainHUD;
	//----------------------------------

	//ShowRole UI
	//----------------------------------
	UPROPERTY(EditAnywhere, Category = Widget, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UShowRole> ShowRoleWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UShowRole> ShowRoleWidget;	
	//----------------------------------

	//BlackBoard UI
	//----------------------------------
	UPROPERTY(EditAnywhere, Category = Widget, meta = (AllowPrivateAccess = true))
	TSubclassOf<class UStartBlackBoard> StartBlackBoardWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStartBlackBoard> StartBlackBoard;
	//----------------------------------
	
	// 추가 부분
public:	
	virtual void SetupInputComponent() override;

	// 맵핑컨테스트 RPC
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_AddInputMapping(UInputMappingContext* ItemMappingContext);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_RemoveInputMapping(UInputMappingContext* ItemMappingContext);

	UFUNCTION(BlueprintCallable)
	void SaveAcquiredItemData(const FItemData& InItemData);

	UFUNCTION(BlueprintCallable)
	const FItemData& GetAcquiredItemData() const;

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_UpdateItemUI(const FItemData& ItemData);

protected:

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);

	UPROPERTY(BlueprintReadOnly)
	FItemData AcquiredItem;

private:
		
	UPROPERTY()
	TObjectPtr<USTAbilitySystemComponent> STAbilitySystemComp;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Input")
	TObjectPtr<USTInputConfig> STInputConfig;

	USTAbilitySystemComponent* GetSTAbilitySystemComponent();

	FString PlayerNickName;

	UPROPERTY()
	TObjectPtr<UPlayerItemSlot> PlayerItemSlot;
	
};

