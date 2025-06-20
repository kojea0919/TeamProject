// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameWork/Test/GameMode/STGameMode.h"
#include "GameFramework/Character.h"
#include "GameFrameWork/MainMap/MainMapGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Character/PlayerState/STPlayerState.h"

void ASTGameMode::GameStart()
{
	if (!MainMapGameState->IsValidLowLevel())
	{
		return;
	}
	
	MainMapGameState->SetRemainSecond(GameProgressTime);

	
	//술래 정하기
	//----------------------------------------------------
	TArray<int32> TaggerIndexArr;
	TaggerIndexArr.Init(0,TaggerNum);
	TaggerIndexArr[0] = FMath::RandRange(0, MaxNumOfPlayers - 1);
	// int32 OtherIndex = FMath::RandRange(0, TaggerNum - 1);
	// while (OtherIndex == TaggerIndexArr[0] )
	// {
	// 	OtherIndex = FMath::RandRange(0, TaggerNum - 1);
	// }
	// TaggerIndexArr[1] = OtherIndex;
	//----------------------------------------------------

	for (int32 idx = 0; idx <  MaxNumOfPlayers; ++idx)
	{
		FVector SpawnLocation;
		if (idx == TaggerIndexArr[0])
		{
			ASTPlayerStateArr[idx]->SetTagger();
			SpawnLocation = TaggerInitLocationArr[0];
		}
		// else if (idx == TaggerIndexArr[1])
		// {
		// 	PlayerStateArray[idx]->SetTagger();
		// 	SpawnLocation = TaggerInitLocationArr[0];
		// }
		else
			SpawnLocation = PlayerStartPositionArr[idx];
		
		if (ACharacter * CurCharacter = GameControllersArray[idx]->GetCharacter())
			CurCharacter->SetActorLocation(SpawnLocation);
	}
}

void ASTGameMode::InitPlayerStartPosition()
{
	int8 Size = GameControllersArray.Num();
	for (int8 idx = 0; idx < Size; ++idx)
	{
		if (ACharacter * Player = GameControllersArray[idx]->GetCharacter())
		{
			Player->SetActorLocation(PlayerStartPositionArr[idx]);
		}
	}
}

void ASTGameMode::BeginPlay()
{
	Super::BeginPlay();

	MainMapGameState = Cast<AMainMapGameState>(UGameplayStatics::GetGameState(this));
}

void ASTGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ASTGameMode::AddPlayerToArray(ASTPlayerState* PlayerState, AController* PlayerController)
{
	ASTPlayerStateArr.AddUnique(PlayerState);
	GameControllersArray.AddUnique(PlayerController);
}


UCharacterClassInfo* ASTGameMode::GetCharacterClassDefaultInfo() const
{
	return ClassDefaults;
}
