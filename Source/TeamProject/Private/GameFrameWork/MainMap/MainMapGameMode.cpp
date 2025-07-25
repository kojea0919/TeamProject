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
#include "Components/CapsuleComponent.h"
#include "GameFrameWork/MainMap/MainMapPlayerState.h"
#include "GameTag/STGamePlayTags.h"
#include "Map/Object/Actor/BaseWeapon.h"
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

	InitModeHUD();
	
	switch (CurGameMode)
	{
	case MissionMode:
		InitMissionModeGame();
		break;
	case HideMode:
		InitHideModeGame();
		break;
	}

	ExitTaggerCnt = 0;
	ExitRunnerCnt = 0;
	CurPlayTaggerCnt = TaggerNum;
	CurPlayRunnerCnt = CurPlayerNum - TaggerNum;

	OnGameStart.Broadcast();
}

void AMainMapGameMode::GameEnd(bool IsTaggerWin)
{
	TaggerCharacterRestoration();
	InitRunner();
	DestroyTagger();

	if (MainMapGameState)
	{
		MainMapGameState->SetCurrentGameState(EGameState::Ready);
		OnGameEnd.Broadcast();
	}

	for (const auto & ControllerInfo : GameControllersMap)
	{
		if (AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(ControllerInfo.Value))
		{
			PlayerController->ShowResult(IsTaggerWin);
		}
	}

	for (const auto & PlayerStateInfo :MainMapPlayerStateMap)
	{
		if (ASTPlayerState * PlayerState = Cast<ASTPlayerState>(PlayerStateInfo.Value))
		{
			PlayerState->InitState();
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

int AMainMapGameMode::IncreaseTaggerStartTime()
{
	CurTaggerStartTime += 10;

	int MaxTime = CurTaggerStartTime >= CurGameProgressTime ? CurGameProgressTime - 10 : MaxTaggerStartTime;
	
	CurTaggerStartTime = FMath::Clamp(CurTaggerStartTime, 0, MaxTime);
	return CurTaggerStartTime;
}

int AMainMapGameMode::DecreaseTaggerStartTime()
{
	CurTaggerStartTime -= 10;
	CurTaggerStartTime = FMath::Clamp(CurTaggerStartTime, 0, MaxTaggerStartTime);
	return CurTaggerStartTime;
}

void AMainMapGameMode::RegisterTagger(class ATaggerCharacter* Tagger)
{
	if (Tagger)
	{
		Taggers.Add(Tagger);
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
				if (MainMapGameState)
				{
					MainMapGameState->IncreasePrisonRunnerNum();
					if (MainMapGameState->GetCurrentGameState() == EGameState::Playing)
					{
						Player->SetActorLocation(CurLocation, false, nullptr, ETeleportType::TeleportPhysics);
						// ARunnerCharacter* Runner = Cast<ARunnerCharacter>(Player);
						// Runner ->InitAbilityActorInfo();
					}
				}
				return;
			}
		}
	}	
}

void AMainMapGameMode::SetGhostMode(ARunnerCharacter* Runner)
{
	if (IsValid(Runner))
	{
		Runner->SetGhostMode();
		Runner->SetActorLocation(PlayerStartPositionArr[0]);
	
		if (MainMapGameState)
			MainMapGameState->IncreaseGhostRunnerNum();
	}
}

void AMainMapGameMode::UpdateAboveGrffitiUI(int Num)
{
	for (auto Controller : GameControllersMap)
	{
		if (AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(Controller.Value))
		{
			PlayerController->UpdateMissionAboveNumber(Num);
		}
	}
}

void AMainMapGameMode::UpdateTotalGraffitiUI()
{
	for (auto Controller : GameControllersMap)
	{
		if (AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(Controller.Value))
		{
			PlayerController->UpdateMissionTotalNumber(CurGraffitiCnt);
		}
	}
}

void AMainMapGameMode::SendRemainChangeTime(int Second)
{
	for (const auto Controller : GameControllersMap)
	{
		if (AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(Controller.Value))
		{
			PlayerController->SetRemainChangeTime(Second);
		}
	}
}

void AMainMapGameMode::ChangeToRandomObject()
{
  	checkf(MainMapGameState,TEXT("AMainMapGameMode::ChangeToRandomObject MainMapGameState is Null"));
	
	for (const auto PlayerStateInfo : MainMapPlayerStateMap)
	{
		ASTPlayerState * CurPlayerState = PlayerStateInfo.Value;
		int CurPlayerServerNumID = PlayerStateInfo.Key;
		if (CurPlayerState && !CurPlayerState->IsPlayerTagger())
		{
			EStaticMeshType ChangeObjectType = MainMapGameState->GetRandomStatieMeshType();
			
			if (AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(GameControllersMap[CurPlayerServerNumID]))
			{
				if (ARunnerCharacter * Runner = Cast<ARunnerCharacter>(PlayerController->GetCharacter()))
				{
					Runner->SetCurrentObjectType(ChangeObjectType);
				}
			}
		}
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

	OnGameStart.AddUFunction(this, FName("UpdateTotalGraffitiUI"));
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

		//Controller, PlayerState를 Map으로 관리
		//-----------------------------------------------------------
		if (IsValid(NewPlayerState) && IsValid(NewPlayer))
		{
			MainMapPlayerStateMap.Add(IDCounter, NewPlayerState);			
			GameControllersMap.Add(IDCounter, NewPlayer);
			NewPlayerState->ServerNumberID = IDCounter;
			IDArr.Add(IDCounter);

			++IDCounter;

			FString NickName = GetSteamNickName(NewPlayerState);
			if (AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(NewPlayer))
			{
				PlayerController->SetPlayerNickName(NickName);
			}
			NewPlayerState->PlayerNickName = NickName;
		}
		//-----------------------------------------------------------
	}
}

void AMainMapGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ASTPlayerState * PlayerState = Exiting->GetPlayerState<ASTPlayerState>();
	if (PlayerState)
	{
		int ServerID = PlayerState->ServerNumberID;
		MainMapPlayerStateMap.Remove(ServerID);
		GameControllersMap.Remove(ServerID);
		IDArr.Remove(ServerID);
	}

	if (AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(Exiting))
	{
		PlayerController->DestroyOriginCharacter();
	}
	
	if (MainMapGameState->GetCurrentGameState() == EGameState::Playing)
	{
		if (PlayerState)
		{
			if (PlayerState->IsPlayerTagger())
				++ExitTaggerCnt;
			else
				++ExitRunnerCnt;
		}
		
		//남은 Player를 확인해서 게임 종료 조건이면 게임 종료
		//-----------------------------------------------		
		if (CurPlayTaggerCnt - ExitTaggerCnt == 0)
			GameEnd(false);
		else if (CurPlayRunnerCnt - ExitRunnerCnt == 0)
			GameEnd(true);
		//-----------------------------------------------
	}
}

void AMainMapGameMode::InitRunner()
{
	int Idx = 0;
	for (auto ControllerInfo : GameControllersMap)
	{
		APlayerController * PlayerController = ControllerInfo.Value;
		if (!IsValid(PlayerController)) continue;
		
		if (ARunnerCharacter * Player = Cast<ARunnerCharacter>(ControllerInfo.Value->GetCharacter()))
		{
			Player->SetActorLocation(PlayerStartPositionArr[Idx]);
			Player->SetActive(true);

			if (CurGameMode == HideMode)
				Player->SetCurrentObjectType(EStaticMeshType::None);
			++Idx;

			Player->InitAbilityActorInfo();
		}
	}

	InitRunnerOutLine(false);
}

void AMainMapGameMode::DestroyTagger()
{
	int8 Size = Taggers.Num();
	for (int Idx = 0; Idx < Size; ++Idx)
	{
	if (IsValid(Taggers[Idx]))
			Taggers[Idx]->Destroy();

		if (ABaseWeapon* Hammer = Taggers[Idx]->GetHammer())
		{
			Hammer->Destroy();
		}
	}
	Taggers.Empty();
}

void AMainMapGameMode::TaggerCharacterRestoration()
{
	for (const auto & CurTaggerController : TaggerController)
	{
		if (CurTaggerController)
			CurTaggerController->PossessOriginCharacter();
	}
}

void AMainMapGameMode::InitGraffiti()
{
	if (USpawnerManagerSubsystem *  Spawner = GetWorld()->GetSubsystem<USpawnerManagerSubsystem>())
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
	Taggers.Empty();
	TaggerController.Empty();
	
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
			
			//Tagger 생성
			if (TaggerCharacterClass)
			{
				if (ATaggerCharacter * NewTagger = GetWorld()->SpawnActor<ATaggerCharacter>(TaggerCharacterClass, TaggerInitLocationArr[Idx],FRotator()))
				{
					CurPlayerController->Possess(NewTagger);
					Taggers.Add(NewTagger);
				}
			}
			TaggerController.Add(CurPlayerController);
		}
		else
		{
			CurCharacter->SetActorLocation(PlayerStartPositionArr[Idx]);
		}		
	}
}

FString AMainMapGameMode::GetSteamNickName(const APlayerState * PlayerState)
{
	FString NickName;
	//Steam NickName 가져오기
	//-----------------------------------------------------------
	if (IOnlineSubsystem * OnlineSub = IOnlineSubsystem::Get(STEAM_SUBSYSTEM))
	{
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		if (Identity.IsValid())
		{
			const FUniqueNetIdRepl & UniqueIdRepl = PlayerState->GetUniqueId();				
			NickName = Identity->GetPlayerNickname(*UniqueIdRepl);
		}
	}
	//-----------------------------------------------------------

	return NickName;
}

void AMainMapGameMode::InitMissionModeGame()
{
	InitGraffiti();	
}

void AMainMapGameMode::InitRunnerOutLine(bool Active)
{
	TArray<ARunnerCharacter *> Runners;
	for (const auto & PlayerControllerInfo : GameControllersMap)
	{
		if (AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(PlayerControllerInfo.Value))
		{
			if (ARunnerCharacter * Runner = Cast<ARunnerCharacter>(PlayerController->GetCharacter()))
			{
				Runners.Add(Runner);
			}
		}
	}

	for (const auto & PlayerControllerInfo : GameControllersMap)
	{
		if (AMainMapPlayerController * PlayerController = Cast<AMainMapPlayerController>(PlayerControllerInfo.Value))
		{
			if (ARunnerCharacter * Runner = Cast<ARunnerCharacter>(PlayerController->GetCharacter()))
			{
				Runner->SetOutLine(Runners,Active);
			}
		}
	}
}

void AMainMapGameMode::InitHideModeGame()
{
	InitRunnerOutLine(true);
}

void AMainMapGameMode::InitModeHUD()
{
	for (const auto & PlayerController : GameControllersMap)
	{
		if (AMainMapPlayerController * MainMapPlayerController = Cast<AMainMapPlayerController>(PlayerController.Value))
		{
			MainMapPlayerController->SetVisibleMainHUD(true);
			MainMapPlayerController->SetGameModeHUD(CurGameMode == MissionMode ? true : false);
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
