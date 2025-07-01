// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Player/Character/Input/STEnhancedInputComponent.h"
#include "Player/Character/PlayerState/STPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "GameFrameWork/MainMap/MainMapPlayerState.h"
#include "UI/MainHUD/PlayerMainHUD.h"
#include "UI/MainHUD/ShowRole.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"

void AMainMapPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		InitInputMode();
		InitWidget();
	}	
}

void AMainMapPlayerController::UpdateRemainTime(int Second)
{
	if (PlayerMainHUD)
	{
		PlayerMainHUD->UpdateRemainTime(Second);
	}
}

void AMainMapPlayerController::SetActiveMic(bool Active)
{
	if (PlayerMainHUD)
		PlayerMainHUD->SetActiveMic(Active);
}

void AMainMapPlayerController::SetTalkingMic()
{
	if (PlayerMainHUD)
		PlayerMainHUD->PlayTalkingAnimation();
}

void AMainMapPlayerController::SendChatMessageServer_Implementation(const FText& Text, EChattingRoomType RoomType)
{
	AMainMapPlayerState * CurPlayerState = Cast<AMainMapPlayerState>(PlayerState);
	if (IsValid(PlayerState))
	{
		FString SendPlayerNickName = CurPlayerState->PlayerNickName;
		
		switch (RoomType)
		{
		case EChattingRoomType::AllChatRoom:
			SendAllChatMessage(Text,SendPlayerNickName);
			break;
		case EChattingRoomType::TeamChatRoom:
			SendTeamChatMessage(Text,SendPlayerNickName);
			break;
		}
		
		//ReceiveChatMessage(Text, SendPlayerNickName);
	}
}

void AMainMapPlayerController::SendAllChatMessage(const FText& Text, const FString& SendPlayerNickName)
{
	if (!HasAuthority())
		return;
	
	AMainMapPlayerState * SendMsgPlayerState = GetPlayerState<AMainMapPlayerState>();
	if (!IsValid(SendMsgPlayerState))
		return;

	int32 SendPlayerServerNumberID = SendMsgPlayerState->ServerNumberID;

	TArray<TObjectPtr<APlayerState>> & PlayerArr = GetWorld()->GetGameState()->PlayerArray;
	int32 Size = PlayerArr.Num();
	
	for (int32 Idx = 0; Idx < Size; ++Idx)
	{
		AMainMapPlayerState * CurPlayerState =  Cast<AMainMapPlayerState>(PlayerArr[Idx]);
		if (!IsValid(CurPlayerState))
			continue;
		
		AMainMapPlayerController * CurPlayerController = Cast<AMainMapPlayerController>(CurPlayerState->GetPlayerController());
		if (!IsValid(CurPlayerController))
			continue;
		
		int32 CurPlayerServerNumbereID = CurPlayerState->ServerNumberID;
		if (SendPlayerServerNumberID == CurPlayerServerNumbereID)
		{
			RecvSelfAllChatMessage(Text);
		}
		else
		{
			CurPlayerController->RecvOtherAllChatMessage(Text,SendPlayerNickName);
		}
	}
}

void AMainMapPlayerController::SendTeamChatMessage(const FText& Text, const FString& SendPlayerNickName)
{
	if (!HasAuthority())
		return;
	
	AMainMapPlayerState * SendMsgPlayerState = GetPlayerState<AMainMapPlayerState>();
	if (!IsValid(SendMsgPlayerState))
		return;

	int32 SendPlayerServerNumberID = SendMsgPlayerState->ServerNumberID;

	TArray<TObjectPtr<APlayerState>> & PlayerArr = GetWorld()->GetGameState()->PlayerArray;
	int32 Size = PlayerArr.Num();
	
	for (int32 Idx = 0; Idx < Size; ++Idx)
	{
		AMainMapPlayerState * CurPlayerState =  Cast<AMainMapPlayerState>(PlayerArr[Idx]);
		if (!IsValid(CurPlayerState))
			continue;
		
		AMainMapPlayerController * CurPlayerController = Cast<AMainMapPlayerController>(CurPlayerState->GetPlayerController());
		if (!IsValid(CurPlayerController))
			continue;
		
		int32 CurPlayerServerNumbereID = CurPlayerState->ServerNumberID;
		if (SendPlayerServerNumberID == CurPlayerServerNumbereID)
		{
			RecvSelfTeamChatMessage(Text);
		}
		else if (CurPlayerState->IsPlayerTargger() == SendMsgPlayerState->IsPlayerTargger())
		{
			CurPlayerController->RecvOtherTeamChatMessage(Text,SendPlayerNickName);
		}
		
	}
}

void AMainMapPlayerController::RecvSelfAllChatMessage_Implementation(const FText& Text)
{
	if (PlayerMainHUD)
	{
		PlayerMainHUD->AddAllChatSelfMessage(Text);
	}
}

void AMainMapPlayerController::RecvOtherAllChatMessage_Implementation(const FText& Text,
	const FString& SendPlayerNickName)
{
	if (PlayerMainHUD)
	{
		PlayerMainHUD->AddAllChatOtherMessage(Text,SendPlayerNickName);
	}
}

void AMainMapPlayerController::RecvSelfTeamChatMessage_Implementation(const FText& Text)
{
	if (PlayerMainHUD)
	{
		PlayerMainHUD->AddTeamChatSelfMessage(Text);
	}
}

void AMainMapPlayerController::RecvOtherTeamChatMessage_Implementation(const FText& Text,
	const FString& SendPlayerNickName)
{
	if (PlayerMainHUD)
	{
		PlayerMainHUD->AddTeamChatOtherMessage(Text, SendPlayerNickName);
	}
}

void AMainMapPlayerController::SetVisibleGameStartUI(bool Visible)
{
	if (PlayerMainHUD)
		PlayerMainHUD->SetVisibleGameStartUI(Visible);
}

void AMainMapPlayerController::ShowRole_Implementation(bool IsTagger)
{	
	if (ShowRoleWidget)
	{
		ShowRoleWidget->ShowRole(IsTagger);
		ShowRoleWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainMapPlayerController::SetJobText_Implementation(bool IsTagger)
{
	if (PlayerMainHUD)
	{
		FString JobStr;
		if (IsTagger)
			JobStr = TEXT("Tagger");
		else
			JobStr = TEXT("Runner");
		PlayerMainHUD->SetPlayerJobText(JobStr);
	}
}

void AMainMapPlayerController::InitInputMode()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AMainMapPlayerController::InitWidget()
{
	if (PlayerMainHUD == nullptr && nullptr != PlayerMainHUDWidgetClass)
	{
		PlayerMainHUD = CreateWidget<UPlayerMainHUD>(this, PlayerMainHUDWidgetClass);
		if (PlayerMainHUD)
		{
			PlayerMainHUD->AddToViewport();
			PlayerMainHUD->Init();
		}
	}

	if (ShowRoleWidget == nullptr && nullptr != ShowRoleWidgetClass)
	{
		ShowRoleWidget = CreateWidget<UShowRole>(this, ShowRoleWidgetClass);
		if (ShowRoleWidget)
		{
			ShowRoleWidget->AddToViewport();
			ShowRoleWidget->SetVisibility(ESlateVisibility::Hidden);
			ShowRoleWidget->Init();
		}
	}
}


// 추가 부분

void AMainMapPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (USTEnhancedInputComponent* STInputComp = Cast<USTEnhancedInputComponent>(InputComponent))
	{
		STInputComp->BindAbilityAction(STInputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);
	}
}

void AMainMapPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if (IsValid(GetSTAbilitySystemComponent()))
	{
		STAbilitySystemComp->AbilityInputPressed(InputTag);
	}
}

void AMainMapPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if (IsValid(GetSTAbilitySystemComponent()))
	{
		STAbilitySystemComp->AbilityInputReleased(InputTag);
	}
}

USTAbilitySystemComponent* AMainMapPlayerController::GetSTAbilitySystemComponent()
{
	if (!IsValid(STAbilitySystemComp))
	{
		if (const ASTPlayerState* STPlayerState = GetPlayerState<ASTPlayerState>())
		{
			STAbilitySystemComp = STPlayerState->GetSTAbilitySystemComponent();
		}
	}
	return STAbilitySystemComp;
}
