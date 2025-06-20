// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/Character/Data/CharacterClassInfo.h"
#include "STGameMode.generated.h"

class AMainMapGameState;
class ASTPlayerState;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ASTGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void AddPlayerStartPosition(const FVector & Position) { PlayerStartPositionArr.Add(Position); }
	
	void SetBlackBoardViewCamera(class ABlackBoardViewCameraActor * CameraActor) { BlackBoardViewCamera = CameraActor; }
	class ABlackBoardViewCameraActor * GetBlackBoardViewCamera() const { return BlackBoardViewCamera; }

	void AddTaggerInitLocation(const FVector & Location) { TaggerInitLocationArr.Add(Location); }

	void GameStart();

	void InitPlayerStartPosition();

protected:
	UPROPERTY(VisibleAnywhere, Category = "System")
	AMainMapGameState* MainMapGameState;

	UPROPERTY(VisibleAnywhere, Category = "System")
	TArray<ASTPlayerState*> ASTPlayerStateArr;

	UPROPERTY(EditDefaultsOnly, Category = "System")
	TArray<AController*> GameControllersArray;

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void AddPlayerToArray(ASTPlayerState* PlayerState, AController* PlayerController);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Mode Setting")
	int32 MaxNumOfPlayers = 3;

	const int32 TaggerNum = 1; 

private:
	TArray<FVector> PlayerStartPositionArr;			//플레이어 Start위치 정보 배열 
	TArray<FVector> TaggerInitLocationArr;			//술래 Start위치 정보 배열
	
	UPROPERTY()
	class ABlackBoardViewCameraActor * BlackBoardViewCamera;

	int GameProgressTime = 27;


	// AbilitySystem 
public:
	UCharacterClassInfo* GetCharacterClassDefaultInfo() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Class Defaults")
	TObjectPtr<UCharacterClassInfo> ClassDefaults;
};
