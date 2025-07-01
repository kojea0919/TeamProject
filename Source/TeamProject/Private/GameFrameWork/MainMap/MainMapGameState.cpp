// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameWork/MainMap/MainMapGameState.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"


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
	if(AMainMapPlayerController * LocalController =
		Cast<AMainMapPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0)))
	{
		switch (CurGameState)
		{
		case EGameState::Ready:
			LocalController->SetVisibleGameStartUI(true);
			break;
		case EGameState::Playing:
			LocalController->SetVisibleGameStartUI(false);
			break;
		default:
			return;
		}
	}
}

void AMainMapGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainMapGameState, RemainSecond);
	DOREPLIFETIME(AMainMapGameState, CurGameState);
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
	if (RemainSecond ==0)
	{
		GetWorldTimerManager().ClearTimer(SecondUpdateTimerHandle);
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
			GameMode->InitPlayerStartPosition();
	}
	//----------------------------------------------------------------
}
