// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMapGameMode.generated.h"

/**
 * 
 */

class AMainMapPlayerState;
class AMainMapGameState;

UCLASS()
class TEAMPROJECT_API AMainMapGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void AddPlayerStartPosition(const FVector & Position) { PlayerStartPositionArr.Add(Position); }
	
	void SetBlackBoardViewCamera(class ABlackBoardViewCameraActor * CameraActor) { BlackBoardViewCamera = CameraActor; }
	class ABlackBoardViewCameraActor * GetBlackBoardViewCamera() const { return BlackBoardViewCamera; }

	void AddTaggerInitLocation(const FVector & Location) { TaggerInitLocationArr.Add(Location); }

	void GameStart();

	void InitPlayerStartPosition();

	int IncreaseGameProgressTime();
	int DecreaseGameProgressTime();

	int IncreaseTaggerCnt();
	int DecreaseTaggerCnt();

	FORCEINLINE int GetTaggerCnt() const { return CurTaggerCnt; }
	FORCEINLINE int GetGameProgressTime() const { return CurGameProgressTime; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Mode Setting")
	int32 MaxNumOfPlayers = 3;

	int32 IDCounter = 0;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "System")
	AMainMapGameState* MainMapGameState;

	UPROPERTY(VisibleAnywhere, Category = "System")
	TMap<int, AMainMapPlayerState*> MainMapPlayerStateMap;
	
	UPROPERTY(EditDefaultsOnly, Category = "System")
	TMap<int, APlayerController*> GameControllersMap;

	TArray<int> IDArr;
	
protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	TArray<FVector> PlayerStartPositionArr;			//플레이어 Start위치 정보 배열 
	TArray<FVector> TaggerInitLocationArr;			//술래 Start위치 정보 배열
	
	UPROPERTY()
	class ABlackBoardViewCameraActor * BlackBoardViewCamera;


	//게임 시작 옵션
	//-----------------------------------------------
	//게임 진행 시간
	int CurGameProgressTime = 70;			
	const int MinGameProgressTime = 10;
	const int MaxGameProgressTime = 600;

	//술래 숫자
	int CurTaggerCnt = 1;
	const int MinTaggerCnt = 1;
	const int MaxTaggerCnt = 2;
	//-----------------------------------------------
};
