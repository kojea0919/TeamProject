// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameWork/MainMap/MainMapGameState.h"

#if WITH_EDITOR
#include "IAutomationControllerManager.h"
#endif
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "GameFrameWork/MainMap/StaticMeshManager/StaticMeshManager.h"
#include "GameFrameWork/MainMap/TaggerBlockBox/TaggerBlockBox.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/MainMap/PrisonCollisionBox/PrisonCollisionBox.h"


AMainMapGameState::AMainMapGameState()
	: RemainSecond(0)
{
	
}

void AMainMapGameState::SetRemainSecond(int Second)
{
	RemainSecond = Second;

	//서버도 UI갱신
	if(AMainMapPlayerController * LocalController =
		Cast<AMainMapPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
		LocalController->UpdateRemainTime(RemainSecond);

	//타이머 시작
	GetWorldTimerManager().SetTimer(SecondUpdateTimerHandle,this,&AMainMapGameState::UpdateSecond,1.f,true);
}

void AMainMapGameState::OnRep_RemainSecond()
{
	if(AMainMapPlayerController * LocalController =
		Cast<AMainMapPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
	{
		LocalController->UpdateRemainTime(RemainSecond);
	}
}

void AMainMapGameState::OnRep_CurGameState()
{
	
}

void AMainMapGameState::OnRep_RemainGraffiti()
{
}

void AMainMapGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainMapGameState, RemainSecond);
	DOREPLIFETIME(AMainMapGameState, CurGameState);
	DOREPLIFETIME(AMainMapGameState, RemainGraffiti);
	DOREPLIFETIME(AMainMapGameState, MaxGraffiti);
	DOREPLIFETIME(AMainMapGameState, StaticMeshManager);
}

void AMainMapGameState::DecreaseGraffitiCount()
{
	if (!HasAuthority())
		return;

	--RemainGraffiti;
	AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>();
	if (GameMode)
	{
		GameMode->UpdateAboveGrffitiUI(RemainGraffiti);
	}
	
	if (RemainGraffiti == 0)
	{
		GameEnd(false);
	}
}

void AMainMapGameState::RegisterPrisonCollisionBox(class APrisonCollisionBox* CollisionBox)
{
	if (PrisonCollisionBox == nullptr)
		PrisonCollisionBox = CollisionBox;
}

void AMainMapGameState::CheckPrision()
{
	if (PrisonCollisionBox)
	{
		PrisonRunnerNum = PrisonCollisionBox->Check();
		
		AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>();
		if (nullptr != GameMode)
		{
			if (PrisonRunnerNum == GameMode->GetRunnerCnt())
			{
				GameEnd(true);
			}
		}
	}		
}

EStaticMeshType AMainMapGameState::GetRandomStatieMeshType()
{
	if (StaticMeshManager)
	{
		int32 RandomType = FMath::RandRange(static_cast<int32>(EStaticMeshType::Desk),
			static_cast<int32>(EStaticMeshType::None)-1);
		return static_cast<EStaticMeshType>(RandomType);
	}
	else
	{
		StaticMeshManager = GetWorld()->SpawnActor<AStaticMeshManager>(StaticMeshManagerClass);
		return EStaticMeshType::Bag;
	}
}

void AMainMapGameState::IncreasePrisonRunnerNum()
{
	++PrisonRunnerNum;

	AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>();
	checkf(GameMode,TEXT("IncreasePrisonRunnerNum MainMapGameMode is null"));

	if (GameMode->GetRunnerCnt() == PrisonRunnerNum)
		GameEnd(true);
}

FStaticMeshInfo AMainMapGameState::GetObjectMesh(EStaticMeshType ObjectType)
{
	if (StaticMeshManager)
	{
		return StaticMeshManager->GetStaticMesh(ObjectType);
	}
	return FStaticMeshInfo();
}

void AMainMapGameState::IncreaseGhostRunnerNum()
{
	AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>();
	if (nullptr != GameMode)
	{
		if (GhostNumber == GameMode->GetRunnerCnt())
		{
			GameEnd(true);
		}
	}
}

void AMainMapGameState::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr == StaticMeshManager && HasAuthority())
	{
		StaticMeshManager = GetWorld()->SpawnActor<AStaticMeshManager>(StaticMeshManagerClass);
	}

	if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
	{
		GameMode->OnGameStart.AddUFunction(this, TEXT("GameStart"));
		GameMode->OnGameEnd.AddUFunction(this,TEXT("GameEnd"));
	}

	TaggerBlockBox = Cast<ATaggerBlockBox>(UGameplayStatics::GetActorOfClass(GetWorld(), ATaggerBlockBox::StaticClass()));
}

void AMainMapGameState::UpdateSecond()
{
	--RemainSecond;	

	//서버도 UI갱신
	if(AMainMapPlayerController * LocalController =
		Cast<AMainMapPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
		LocalController->UpdateRemainTime(RemainSecond);

	//게임 종료
	//플레이어들을 다시 시작 장소로
	//----------------------------------------------------------------
	if (RemainSecond == 0)
	{
		GetWorldTimerManager().ClearTimer(SecondUpdateTimerHandle);

		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			GameEnd(GameMode->GetCurrentGameMode() == MissionMode ? true : false);
		}
	}
	//----------------------------------------------------------------
}

void AMainMapGameState::UpdateTaggerStartTime()
{
	if (TaggerBlockBox)
	{
		TaggerBlockBox->SetBlock(false);
	}
}

void AMainMapGameState::UpdateChangeTime()
{
	if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
	{
		GameMode->SendRemainChangeTime(CurRemainChangeTime--);
		if (CurRemainChangeTime == -1)
		{
			GetWorldTimerManager().ClearTimer(ChangeObjectTimerHandle);
			GameMode->ChangeToRandomObject();
		}
	}
}

void AMainMapGameState::GameStart()
{
	CurGameState = EGameState::Playing;

	if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
	{
		int TaggerStartTime = GameMode->GetTaggerStartTime();
		if (TaggerStartTime != 0)
		{
			if (TaggerBlockBox)
			{
				TaggerBlockBox->SetBlock(true);
				GetWorldTimerManager().SetTimer(TaggerStartTimerHandle, this, &AMainMapGameState::UpdateTaggerStartTime, TaggerStartTime, false);
			}
		}
		else
		{
			if (TaggerBlockBox)
				TaggerBlockBox->SetBlock(false);
		}

		if (GameMode->GetCurrentGameMode() == HideMode)
		{
			GetWorldTimerManager().SetTimer(ChangeObjectTimerHandle, this, &AMainMapGameState::UpdateChangeTime, 1.0f, true);
			GhostNumber = 0;
		}
	}
}

void AMainMapGameState::GameEnd()
{
	GetWorldTimerManager().ClearTimer(SecondUpdateTimerHandle);

	RemainSecond = 0;

	//서버도 UI갱신
	if(AMainMapPlayerController * LocalController =
		Cast<AMainMapPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
		LocalController->UpdateRemainTime(RemainSecond);
}

void AMainMapGameState::GameEnd(bool IsTaggerWin)
{
	if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		GameMode->GameEnd(IsTaggerWin);
		
	RemainSecond = 0;
	GetWorldTimerManager().ClearTimer(SecondUpdateTimerHandle);

	if(AMainMapPlayerController * LocalController =
		Cast<AMainMapPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
		LocalController->UpdateRemainTime(RemainSecond);

	if (TaggerBlockBox)
		TaggerBlockBox->SetBlock(true);
}
