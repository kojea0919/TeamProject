#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "GameFramework/Character.h"
#include "GameFrameWork/MainMap/MainMapGameState.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Player/Character/BaseCharacter.h"
#include "Player/Character/TaggerCharacter.h"
#include "Player/Character/RunnerCharacter.h"
#include "Player/Character/PlayerState/STPlayerState.h"

void AMainMapGameMode::GameStart()
{
	int32 CurPlayerNum = IDArr.Num();	
	if (!MainMapGameState->IsValidLowLevel() || CurPlayerNum == 1)
	{
		return;
	}
	
	MainMapGameState->SetRemainSecond(CurGameProgressTime);
	
	//Select Tagger
	//----------------------------------------------------
	int32 CurTaggerCount = 0;
	TArray<bool> TaggerArr;
	TaggerArr.Init(false,IDArr.Num());
	
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
	CharacterMap.Empty();
	
	int TaggerIdx = 0;
	for (int Idx = 0; Idx < CurPlayerNum; ++Idx)
	{
		int32 CurPlayerServerNumberID = IDArr[Idx];
		bool IsTagger = TaggerArr[Idx];

		ASTPlayerState * CurPlayerState = MainMapPlayerStateMap[CurPlayerServerNumberID];
		AMainMapPlayerController * CurPlayerController = Cast<AMainMapPlayerController>(GameControllersMap[CurPlayerServerNumberID]);
		ARunnerCharacter * CurCharacter = Cast<ARunnerCharacter>(CurPlayerController->GetCharacter());
		
		if (!IsValid(CurPlayerState) || !IsValid(CurPlayerController) ||
			!IsValid(CurCharacter)) continue;

		CurPlayerController->ShowRole(IsTagger);
		CurPlayerController->SetJobText(IsTagger);
		
		if (IsTagger)
		{
			CurPlayerState->SetTagger();
			CurCharacter->SetActive(false);

			CharacterMap.Add(Taggers[TaggerIdx], CurCharacter);
			CurPlayerController->Possess(Taggers[TaggerIdx++]);
						
			//Tagger가 전부 배정된 경우 나머지 Tagger Active false
			//--------------------------------------------------
			int8 Num = Taggers.Num();
			if (TaggerIdx == TaggerNum)
			{
				for (int i = TaggerIdx; i < Num; ++i)
				{
					Taggers[i]->SetActive(false);
				}
			}
			//--------------------------------------------------
		}
		else
		{
			CurCharacter->SetActorLocation(PlayerStartPositionArr[Idx]);
		}		
	}
	//----------------------------------------------------

	MainMapGameState->SetCurrentGameState(EGameState::Playing);
}

void AMainMapGameMode::GameEnd()
{
	TaggerCharacterRestoration();
	InitRunnerStartPosition();
	InitTaggerStartPosition();

	if (MainMapGameState)
		MainMapGameState->SetCurrentGameState(EGameState::Ready);
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

void AMainMapGameMode::RegisterTagger(class ATaggerCharacter* Tagger)
{
	if (Tagger)
	{
		Taggers.Add(Tagger);
	}
}

void AMainMapGameMode::RegisterRunner(class ARunnerCharacter* Runner)
{
	if (Runner)
	{
		Runners.Add(Runner);
	}
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
		ASTPlayerState* NewPlayerState = Cast<ASTPlayerState>(NewPlayer->PlayerState);
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

void AMainMapGameMode::InitRunnerStartPosition()
{
	int8 Size = GameControllersMap.Num();
	int Idx = 0;
	for (auto ControllerInfo : GameControllersMap)
	{
		APlayerController * PlayerController = ControllerInfo.Value;
		if (!IsValid(PlayerController)) return;
		
		if (ARunnerCharacter * Player = Cast<ARunnerCharacter>(ControllerInfo.Value->GetCharacter()))
		{
			Player->SetActorLocation(PlayerStartPositionArr[Idx]);
			Player->SetActive(true);
			++Idx;
		}
	}		
}

void AMainMapGameMode::InitTaggerStartPosition()
{
	int8 Size = Taggers.Num();
	for (int Idx = 0; Idx < Size; ++Idx)
	{
		if (Taggers[Idx])
		{
			Taggers[Idx]->SetActorLocation(TaggerInitLocationArr[Idx]);
			Taggers[Idx]->SetActive(true);
		}
	}		
}

void AMainMapGameMode::TaggerCharacterRestoration()
{
	for (const TPair<ATaggerCharacter*,ARunnerCharacter*> & CharacterPair : CharacterMap)
	{
		ATaggerCharacter * CurTaggerCharacter = CharacterPair.Key;
		ARunnerCharacter * TargetRunnerCharacter = CharacterPair.Value;

		AMainMapPlayerController * MainMapPlayerController =
			CurTaggerCharacter->GetController<AMainMapPlayerController>();

		MainMapPlayerController->Possess(TargetRunnerCharacter);
	}
}

