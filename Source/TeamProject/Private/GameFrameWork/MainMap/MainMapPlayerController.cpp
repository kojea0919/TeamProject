// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Player/Character/Input/STEnhancedInputComponent.h"
#include "Player/Character/PlayerState/STPlayerState.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "UI/MainHUD/PlayerMainHUD.h"
#include "UI/MainHUD/ShowRole.h"
#include "UI/MainHUD/ShowResult.h"
#include "UI/BlackBoard/StartBlackBoard.h"
#include "Net/UnrealNetwork.h"

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

void AMainMapPlayerController::SendChatMessageServer_Implementation(const FChatType & ChatType, EChattingRoomType RoomType)
{
	ASTPlayerState * CurPlayerState = Cast<ASTPlayerState>(PlayerState);
	if (IsValid(PlayerState))
	{
		FString SendPlayerNickName = CurPlayerState->PlayerNickName;
		
		switch (RoomType)
		{
		case EChattingRoomType::AllChatRoom:
			SendAllChatMessage(ChatType,SendPlayerNickName);
			break;
		case EChattingRoomType::TeamChatRoom:
			SendTeamChatMessage(ChatType,SendPlayerNickName);
			break;
		}
	}
}

void AMainMapPlayerController::SendAllChatMessage(const FChatType & ChatType, const FString& SendPlayerNickName)
{
	if (!HasAuthority())
		return;
	
	ASTPlayerState * SendMsgPlayerState = GetPlayerState<ASTPlayerState>();
	if (!IsValid(SendMsgPlayerState))
		return;

	int32 SendPlayerServerNumberID = SendMsgPlayerState->ServerNumberID;

	TArray<TObjectPtr<APlayerState>> & PlayerArr = GetWorld()->GetGameState()->PlayerArray;
	int32 Size = PlayerArr.Num();
	
	for (int32 Idx = 0; Idx < Size; ++Idx)
	{
		ASTPlayerState * CurPlayerState =  Cast<ASTPlayerState>(PlayerArr[Idx]);
		if (!IsValid(CurPlayerState))
			continue;
		
		AMainMapPlayerController * CurPlayerController = Cast<AMainMapPlayerController>(CurPlayerState->GetPlayerController());
		if (!IsValid(CurPlayerController))
			continue;
		
		int32 CurPlayerServerNumbereID = CurPlayerState->ServerNumberID;
		if (SendPlayerServerNumberID == CurPlayerServerNumbereID)
		{
			RecvSelfAllChatMessage(ChatType);
		}
		else
		{
			CurPlayerController->RecvOtherAllChatMessage(ChatType,SendPlayerNickName);
		}
	}
}

void AMainMapPlayerController::SendTeamChatMessage(const FChatType & ChatType, const FString& SendPlayerNickName)
{
	if (!HasAuthority())
		return;
	
	ASTPlayerState * SendMsgPlayerState = GetPlayerState<ASTPlayerState>();
	if (!IsValid(SendMsgPlayerState))
		return;

	int32 SendPlayerServerNumberID = SendMsgPlayerState->ServerNumberID;

	TArray<TObjectPtr<APlayerState>> & PlayerArr = GetWorld()->GetGameState()->PlayerArray;
	int32 Size = PlayerArr.Num();
	
	for (int32 Idx = 0; Idx < Size; ++Idx)
	{
		ASTPlayerState * CurPlayerState =  Cast<ASTPlayerState>(PlayerArr[Idx]);
		if (!IsValid(CurPlayerState))
			continue;
		
		AMainMapPlayerController * CurPlayerController = Cast<AMainMapPlayerController>(CurPlayerState->GetPlayerController());
		if (!IsValid(CurPlayerController))
			continue;
		
		int32 CurPlayerServerNumbereID = CurPlayerState->ServerNumberID;
		if (SendPlayerServerNumberID == CurPlayerServerNumbereID)
		{
			RecvSelfTeamChatMessage(ChatType);
		}
		else if (CurPlayerState->IsPlayerTargger() == SendMsgPlayerState->IsPlayerTargger())
		{
			CurPlayerController->RecvOtherTeamChatMessage(ChatType,SendPlayerNickName);
		}
		
	}
}

void AMainMapPlayerController::RecvSelfAllChatMessage_Implementation(const FChatType & ChatType)
{
	if (PlayerMainHUD)
	{
		PlayerMainHUD->AddAllChatSelfMessage(ChatType);
	}
}

void AMainMapPlayerController::RecvOtherAllChatMessage_Implementation(const FChatType & ChatType,
	const FString& SendPlayerNickName)
{
	if (PlayerMainHUD)
	{
		PlayerMainHUD->AddAllChatOtherMessage(ChatType,SendPlayerNickName);
	}
}

void AMainMapPlayerController::RecvSelfTeamChatMessage_Implementation(const FChatType & ChatType)
{
	if (PlayerMainHUD)
	{
		PlayerMainHUD->AddTeamChatSelfMessage(ChatType);
	}
}

void AMainMapPlayerController::RecvOtherTeamChatMessage_Implementation(const FChatType & ChatType,
	const FString& SendPlayerNickName)
{
	if (PlayerMainHUD)
	{
		PlayerMainHUD->AddTeamChatOtherMessage(ChatType, SendPlayerNickName);
	}
}

void AMainMapPlayerController::ShowRole_Implementation(bool IsTagger)
{	
	if (ShowRoleWidget)
	{
		ShowRoleWidget->ShowRole(IsTagger);
		ShowRoleWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainMapPlayerController::ShowResult_Implementation(bool IsTaggerWin)
{
	if (ShowResultWidget)
	{
		ShowResultWidget->ShowResult(IsTaggerWin);
		ShowResultWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainMapPlayerController::OnRep_PlayerNickname()
{
	if (PlayerMainHUD)
		PlayerMainHUD->SetPlayerNickName(PlayerNickName);
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

void AMainMapPlayerController::SetVisibleBlackBoard(bool Visible)
{
	if (nullptr == StartBlackBoard)
		return;

	if (Visible)
	{
		StartBlackBoard->SetVisibility(ESlateVisibility::Visible);
		StartBlackBoard->PlayFadeIn();
	}
	else
	{
		StartBlackBoard->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void AMainMapPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainMapPlayerController, PlayerNickName);
}

void AMainMapPlayerController::ClearSmartPhone_Implementation()
{
	if (IsLocalController() && PlayerMainHUD)
	{
		PlayerMainHUD->ClearSmartPhone();
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

	if (StartBlackBoard == nullptr && nullptr != StartBlackBoardWidgetClass)
	{
		StartBlackBoard = CreateWidget<UStartBlackBoard>(this, StartBlackBoardWidgetClass);
		if (StartBlackBoard)
		{
			StartBlackBoard->AddToViewport();
			StartBlackBoard->SetVisibility(ESlateVisibility::Hidden);
			StartBlackBoard->Init();
		}
	}

	if (ShowResultWidget == nullptr && nullptr != ShowResultWidgetClass)
	{
		ShowResultWidget = CreateWidget<UShowResult>(this, ShowResultWidgetClass);
		if (ShowResultWidget)
		{
			ShowResultWidget->AddToViewport();
			ShowResultWidget->SetVisibility(ESlateVisibility::Hidden);
			ShowResultWidget->Init();
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
