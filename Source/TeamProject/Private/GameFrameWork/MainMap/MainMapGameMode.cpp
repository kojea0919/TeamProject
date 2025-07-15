#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "GameFramework/Character.h"
#include "GameFrameWork/MainMap/MainMapGameState.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Player/Character/BaseCharacter.h"
#include "Player/Character/TaggerCharacter.h"
#include "Player/Character/RunnerCharacter.h"
#include "Player/Character/PlayerState/STPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Elements/Framework/TypedElementQueryBuilder.h"
#include "GameTag/STGamePlayTags.h"
#include "Map/Object/Subsystem/WorldSubsystem/SpawnerManagerSubsystem.h"

void AMainMapGameMode::GameStart()
{
	int32 CurPlayerNum = IDArr.Num();
	if (!MainMapGameState->IsValidLowLevel() || CurPlayerNum <= 1)
	{
		return;
	}
	
	MainMapGameState->SetRemainSecond(CurGameProgressTime);
	
	//Select Tagger
	//----------------------------------------------------
	TArray<bool> TaggerArr;
	int TaggerNum = FMath::Clamp(CurTaggerCnt, 1,CurPlayerNum - 1);
	SelectTagger(TaggerNum,TaggerArr, CurPlayerNum);
	//----------------------------------------------------

	//Spawn Player
	//----------------------------------------------------
	SpawnPlayer(TaggerNum,TaggerArr,CurPlayerNum);
	//----------------------------------------------------

	InitGraffiti();

	if (MainMapGameState)
	{
		MainMapGameState->SetCurrentGameState(EGameState::Playing);
		OnGameStart.Broadcast();
	}
}

void AMainMapGameMode::GameEnd()
{
	TaggerCharacterRestoration();
	InitRunnerStartPosition();
	InitTaggerStartPosition();

	if (MainMapGameState)
	{
		MainMapGameState->SetCurrentGameState(EGameState::Ready);
		OnGameEnd.Broadcast();
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

int AMainMapGameMode::IncreaseGraffitiCnt()
{
	++CurGraffitiCnt;
	CurGraffitiCnt = FMath::Clamp(CurGraffitiCnt, MinGraffitiCnt, MaxGraffitiCnt);

	return CurGraffitiCnt;
}

int AMainMapGameMode::DecreaseGraffitiCnt()
{
	--CurGraffitiCnt;
	CurGraffitiCnt = FMath::Clamp(CurGraffitiCnt, MinGraffitiCnt, MaxGraffitiCnt);

	return CurGraffitiCnt;
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

void AMainMapGameMode::SendToPrison(class ACharacter* Player)
{
	if (!IsValid(Player))
		return;
	
	if (UCapsuleComponent * Capsule = Player->GetCapsuleComponent())
	{
		int32 Num = PrisonSpawnLocationArr.Num();
		if (Num == 0)
			return;
		
		for (int32 Idx = 0; Idx < Num; ++Idx)
		{
			const FVector & CurLocation = PrisonSpawnLocationArr[Idx];
	
			FCollisionShape Shape = Capsule->GetCollisionShape();
			bool IsBlock =  GetWorld()->OverlapBlockingTestByChannel(
				CurLocation,
				FQuat::Identity,
				Capsule->GetCollisionObjectType(),
				Shape);
	
			if (!IsBlock)
			{
				Player->SetActorLocation(CurLocation);
				return;
			}
		}
	
		Player->SetActorLocation(PrisonSpawnLocationArr[0]);
	}	
}

void AMainMapGameMode::BeginPlay()
{
	Super::BeginPlay();

	MainMapGameState = Cast<AMainMapGameState>(UGameplayStatics::GetGameState(this));

	OnGameEnd.AddLambda([this]()
	{
		for (auto Controller : GameControllersMap)
		{
			if (AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(Controller.Value))
			{
				PlayerController->ClearSmartPhone();
			}
		}
	});
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

void AMainMapGameMode::InitGraffiti()
{
	USpawnerManagerSubsystem *  Spawner = GetWorld()->GetSubsystem<USpawnerManagerSubsystem>();
	if (Spawner)
	{
		Spawner->ClearSpawnRequestData();
		Spawner->AddSpawnRequestData(STGamePlayTags::Object_Actor_Graffiti,CurGraffitiCnt);
		Spawner->ExecuteSpawnRequests();
	}

	if (MainMapGameState)
		MainMapGameState->SetRemainGraffitiCount(CurGraffitiCnt);
}

void AMainMapGameMode::SpawnPlayer(int TaggerNum, const TArray<bool>& TaggerArr, int CurPlayerNum)
{
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
}

void AMainMapGameMode::SelectTagger(int TaggerNum,TArray<bool> & TaggerArr,int CurPlayerNum) const
{
	int32 CurTaggerCount = 0;
	TaggerArr.Init(false,IDArr.Num());
	
	
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
}

UCharacterClassInfo* AMainMapGameMode::GetCharacterClassDefaultInfo() const
{
	return ClassDefaults;
}
