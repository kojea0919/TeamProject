#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "GameFramework/Character.h"
#include "GameFrameWork/MainMap/MainMapPlayerState.h"
#include "GameFrameWork/MainMap/MainMapGameState.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

void AMainMapGameMode::GameStart()
{
	if (!MainMapGameState->IsValidLowLevel())
	{
		return;
	}
	
	MainMapGameState->SetRemainSecond(CurGameProgressTime);
	
	//Select Tagger
	//----------------------------------------------------
	int32 CurTaggerCount = 0;
	TArray<bool> TaggerArr;
	TaggerArr.Init(false,IDArr.Num());
	int32 CurPlayerNum = IDArr.Num();
	if (CurPlayerNum == 1)
		return;
	
	int TaggerNum = FMath::Clamp(CurTaggerCnt, 1,CurPlayerNum - 1);
	
	while (CurTaggerCount < TaggerNum)
	{
		int32 CurRandomIdx = FMath::RandRange(0,CurPlayerNum - 1);
		if (TaggerArr[CurRandomIdx])
			continue;
		else
		{
			TaggerArr[CurRandomIdx] = true;
			++CurTaggerCount;
		}		
	}
	//----------------------------------------------------

	//Spawn Player
	//----------------------------------------------------
	for (int Idx = 0; Idx < CurPlayerNum; ++Idx)
	{
		int32 CurPlayerServerNumberID = IDArr[Idx];
		bool IsTagger = TaggerArr[Idx];

		AMainMapPlayerState * CurPlayerState = MainMapPlayerStateMap[CurPlayerServerNumberID];
		AMainMapPlayerController * CurPlayerController = Cast<AMainMapPlayerController>(GameControllersMap[CurPlayerServerNumberID]);
		ACharacter * CurCharacter = CurPlayerController->GetCharacter();
		
		if (!IsValid(CurPlayerState) || !IsValid(CurPlayerController) ||
			!IsValid(CurCharacter)) continue;

		CurPlayerController->ShowRole(IsTagger);
		CurPlayerController->SetJobText(IsTagger);
		
		if (IsTagger)
		{
			CurPlayerState->SetTagger();
			CurCharacter->SetActorLocation(TaggerInitLocationArr[Idx]);			
		}
		else
		{
			CurCharacter->SetActorLocation(PlayerStartPositionArr[Idx]);
		}		
	}
	//----------------------------------------------------

	MainMapGameState->SetCurrentGameState(EGameState::Playing);
	
	
}

void AMainMapGameMode::InitPlayerStartPosition()
{
	int8 Size = GameControllersMap.Num();
	int Idx = 0;
	for (auto ControllerInfo : GameControllersMap)
	{
		APlayerController * PlayerController = ControllerInfo.Value;
		if (!IsValid(PlayerController)) return;
		
		if (ACharacter * Player = ControllerInfo.Value->GetCharacter())
		{
			Player->SetActorLocation(PlayerStartPositionArr[Idx]);
			++Idx;
		}
	}		
}

int AMainMapGameMode::IncreaseGameProgressTime()
{
	CurGameProgressTime += 10;
	CurGameProgressTime = FMath::Clamp(CurGameProgressTime, MinGameProgressTime, MaxGameProgressTime);

	return CurGameProgressTime;
}

int AMainMapGameMode::DecreaseGameProgressTime()
{
	CurGameProgressTime -= 10;
	CurGameProgressTime = FMath::Clamp(CurGameProgressTime, MinGameProgressTime, MaxGameProgressTime);

	return CurGameProgressTime;
}

int AMainMapGameMode::IncreaseTaggerCnt()
{
	++CurTaggerCnt;
	CurTaggerCnt = FMath::Clamp(CurTaggerCnt, MinTaggerCnt,MaxTaggerCnt);

	return CurTaggerCnt;
}

int AMainMapGameMode::DecreaseTaggerCnt()
{
	--CurTaggerCnt;
	CurTaggerCnt = FMath::Clamp(CurTaggerCnt, MinTaggerCnt,MaxTaggerCnt);

	return CurTaggerCnt;
}

void AMainMapGameMode::BeginPlay()
{
	Super::BeginPlay();

	MainMapGameState = Cast<AMainMapGameState>(UGameplayStatics::GetGameState(this));
}

void AMainMapGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	if (GameControllersMap.Num() < MaxNumOfPlayers)
	{
		AMainMapPlayerState* NewPlayerState = Cast<AMainMapPlayerState>(NewPlayer->PlayerState);
		if (!IsValid(NewPlayerState))
		{
			UE_LOG(LogTemp,Warning,TEXT("AMainMapGameMode::PostLogin NewPlayerState Null"));
			return;
		}
		
		if (IsValid(NewPlayerState) && IsValid(NewPlayer))
		{
			MainMapPlayerStateMap.Add(IDCounter, NewPlayerState);			
			GameControllersMap.Add(IDCounter, NewPlayer);
			NewPlayerState->ServerNumberID = MainMapPlayerStateMap.Num();
			IDArr.Add(IDCounter);

			NewPlayerState->PlayerNickName = FString(TEXT("학생")) + FString::FromInt(IDCounter);
			
			++IDCounter;
		}

		if (IOnlineSubsystem * OnlineSub = IOnlineSubsystem::Get(STEAM_SUBSYSTEM))
		{
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
			if (Identity.IsValid())
			{
				const FUniqueNetIdRepl & UniqueIdRepl = NewPlayer->PlayerState->GetUniqueId();				
				FString Nickname = Identity->GetPlayerNickname(*UniqueIdRepl);

				if (AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(NewPlayer))
				{
					PlayerController->SetPlayerNickName(Nickname);
				}
			}
		}
	}
}

