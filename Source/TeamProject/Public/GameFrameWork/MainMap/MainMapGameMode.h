#pragma once

#include "CoreMinimal.h"
#include "AI/NavigationSystemBase.h"
#include "GameFramework/GameModeBase.h"
#include "Player/Character/Data/CharacterClassInfo.h"
#include "MainMapGameMode.generated.h"

/**
 * 
 */



class ASTPlayerState;
class AMainMapGameState;

UENUM(BlueprintType)
enum EGameMode
{
	MissionMode,
	HideMode
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStart, EGameMode);
DECLARE_MULTICAST_DELEGATE(FOnGameEnd);

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
	void GameEnd(bool IsTaggerWin);

	int IncreaseGameProgressTime();
	int DecreaseGameProgressTime();

	int IncreaseTaggerCnt();
	int DecreaseTaggerCnt();

	int IncreaseGraffitiCnt();
	int DecreaseGraffitiCnt();

	int IncreaseTaggerStartTime();
	int DecreaseTaggerStartTime();

	FORCEINLINE int GetTaggerCnt() const { return CurTaggerCnt; }
	FORCEINLINE int GetGameProgressTime() const { return CurGameProgressTime; }
	FORCEINLINE int GetGraffitiCnt() const { return CurGraffitiCnt; }
	FORCEINLINE int GetTaggerStartTime() const { return CurTaggerStartTime; }
	
	void RegisterTagger(class ATaggerCharacter * Tagger);

	UFUNCTION(BlueprintCallable)
	void SendToPrison(class ACharacter * Player);

	UFUNCTION(BlueprintCallable)
	void SetGhostMode(class ARunnerCharacter * Runner);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGameMode GetCurrentGameMode() const { return CurGameMode; }
	FORCEINLINE void SetCurrentGameMode(bool IsMissionMode) { IsMissionMode == true ? CurGameMode = MissionMode : CurGameMode = HideMode; } 
	
	void UpdateAboveGrffitiUI(int Num);

	UFUNCTION()
	void UpdateTotalGraffitiUI();

	FORCEINLINE int GetRunnerCnt() const { return IDArr.Num() - CurTaggerCnt; }

	void SendRemainChangeTime(int Second);
	void ChangeToRandomObject();
	
public:
	FOnGameStart OnGameStart;
	FOnGameEnd OnGameEnd;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Mode Setting")
	int32 MaxNumOfPlayers = 6;

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
	virtual void Logout(AController* Exiting) override;
	
private:
	void InitRunner();
	void DestroyTagger();
	void TaggerCharacterRestoration();
	void InitGraffiti();
	void SelectTagger(int TaggerNum,TArray<bool> & TaggerArr,int CurPlayerNum) const;
	void SpawnPlayer(int TaggerNum, const TArray<bool> & TaggerArr,int CurPlayerNum);
	FString GetSteamNickName(const APlayerState * PlayerState);

	void InitMissionModeGame();
	void InitRunnerOutLine(bool Active);
	void InitHideModeGame();

	void InitModeHUD();
private:
	TArray<FVector> PlayerStartPositionArr;			//플레이어 Start위치 정보 배열 
	TArray<FVector> TaggerInitLocationArr;			//술래 Start위치 정보 배열
	TArray<FVector> PrisonSpawnLocationArr;			//감옥 스폰위치 정보 배열
	
	UPROPERTY()
	TObjectPtr<ABlackBoardViewCameraActor> BlackBoardViewCamera;	

	//게임 시작 옵션
	//-----------------------------------------------
	//게임 진행 시간
	int CurGameProgressTime = 70;			
	const int MinGameProgressTime = 10;
	const int MaxGameProgressTime = 600;

	//술래 숫자
	int CurTaggerCnt = 1;
	const int MinTaggerCnt = 1;
	const int MaxTaggerCnt = 5;

	//낙서 숫자
	int CurGraffitiCnt = 5;
	const int MinGraffitiCnt = 1;
	const int MaxGraffitiCnt = 30;

	//현재 게임에서 나간 플레이어 숫자
	int ExitTaggerCnt = 0;
	int ExitRunnerCnt = 0;

	//현재 게임이 시작 됐을 때 플레이어 숫자
	int CurPlayTaggerCnt = 0;
	int CurPlayRunnerCnt = 0;

	//게임 모드
	EGameMode CurGameMode = MissionMode;

	//술래 Start시간
	int CurTaggerStartTime = 30;
	const int MaxTaggerStartTime = 60;
	//-----------------------------------------------

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATaggerCharacter> TaggerCharacterClass;
	
	UPROPERTY()
	TArray<class ATaggerCharacter*> Taggers;

	UPROPERTY()
	TArray<class AMainMapPlayerController*> TaggerController;
	
	// AbilitySystem 
public:
	UCharacterClassInfo* GetCharacterClassDefaultInfo() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Class Defaults")
	TObjectPtr<UCharacterClassInfo> ClassDefaults;
};
