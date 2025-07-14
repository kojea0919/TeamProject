#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/Character/Data/CharacterClassInfo.h"
#include "MainMapGameMode.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnGameStart);
DECLARE_MULTICAST_DELEGATE(FOnGameEnd);

class ASTPlayerState;
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
	void AddPrisonSpawnLocation(const FVector & Location) { PrisonSpawnLocationArr.Add(Location); }
	
	void GameStart();
	void GameEnd();

	int IncreaseGameProgressTime();
	int DecreaseGameProgressTime();

	int IncreaseTaggerCnt();
	int DecreaseTaggerCnt();

	int IncreaseGraffitiCnt();
	int DecreaseGraffitiCnt();

	FORCEINLINE int GetTaggerCnt() const { return CurTaggerCnt; }
	FORCEINLINE int GetGameProgressTime() const { return CurGameProgressTime; }
	FORCEINLINE int GetGraffitiCnt() const { return CurGraffitiCnt; }

	void RegisterTagger(class ATaggerCharacter * Tagger);
	void RegisterRunner(class ARunnerCharacter * Runner);

	UFUNCTION(BlueprintCallable)
	void SendToPrison(class ACharacter * Player);
	
public:
	FOnGameStart OnGameStart;
	FOnGameEnd OnGameEnd;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Mode Setting")
	int32 MaxNumOfPlayers = 3;

	int32 IDCounter = 0;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "System")
	AMainMapGameState* MainMapGameState;

	UPROPERTY(VisibleAnywhere, Category = "System")
	TMap<int, ASTPlayerState*> MainMapPlayerStateMap;
	
	UPROPERTY(EditDefaultsOnly, Category = "System")
	TMap<int, APlayerController*> GameControllersMap;

	TArray<int> IDArr;
	
protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	void InitRunnerStartPosition();
	void InitTaggerStartPosition();
	void TaggerCharacterRestoration();
	
private:
	TArray<FVector> PlayerStartPositionArr;			//플레이어 Start위치 정보 배열 
	TArray<FVector> TaggerInitLocationArr;			//술래 Start위치 정보 배열
	TArray<FVector> PrisonSpawnLocationArr;			//감옥 스폰위치 정보 배열
	
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

	//낙서 숫자
	int CurGraffitiCnt = 5;
	const int MinGraffitiCnt = 1;
	const int MaxGraffitiCnt = 99;
	//-----------------------------------------------

	UPROPERTY()
	TArray<class ATaggerCharacter*> Taggers;

	UPROPERTY()
	TArray<class ARunnerCharacter*> Runners;

	UPROPERTY()
	TMap<class ATaggerCharacter*, class ARunnerCharacter*> CharacterMap;


	// AbilitySystem 
public:
	UCharacterClassInfo* GetCharacterClassDefaultInfo() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Class Defaults")
	TObjectPtr<UCharacterClassInfo> ClassDefaults;
};
