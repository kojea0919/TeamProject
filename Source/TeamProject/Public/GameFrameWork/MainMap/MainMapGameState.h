// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MainMapGameState.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Ready,
	Playing
};


/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AMainMapGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMainMapGameState();

public:
	void SetRemainSecond(int Second);	//Second 갱신하면서 UI도 Update

	UFUNCTION()
	void OnRep_RemainSecond();

	UFUNCTION()
	void OnRep_CurGameState();

	UFUNCTION()
	void OnRep_RemainGraffiti();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGameState GetCurrentGameState() const { return CurGameState;}

	FORCEINLINE void SetCurrentGameState(EGameState NewGameState) { CurGameState = NewGameState; }

	void DecreaseGraffitiCount();

public:
	FORCEINLINE void SetMaxGraffitiCount(int Count) { MaxGraffiti = Count; }
	FORCEINLINE void SetRemainGraffitiCount(int Count) { RemainGraffiti = Count; }
	FORCEINLINE int GetMaxGraffitiCount() const { return MaxGraffiti; }
	FORCEINLINE int GetRemainGraffiti() const { return RemainGraffiti; }

protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void UpdateSecond();

	UFUNCTION()
	void UpdateTaggerStartTime();
	
	UFUNCTION()
	void GameStart();

	UFUNCTION()
	void GameEnd();
	
	void GameEnd(bool IsTaggerWin);
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_RemainSecond)
	int RemainSecond;

	FTimerHandle SecondUpdateTimerHandle;

	UPROPERTY(ReplicatedUsing = OnRep_CurGameState)
	EGameState CurGameState;

	UPROPERTY(ReplicatedUsing = OnRep_RemainGraffiti)
	int RemainGraffiti;

	UPROPERTY(Replicated)
	int MaxGraffiti;

	UPROPERTY(Replicated)
	TObjectPtr<class AStaticMeshManager> StaticMeshManager;

	UPROPERTY()
	TObjectPtr<class ATaggerBlockBox> TaggerBlockBox;

	FTimerHandle TaggerStartTimerHandle;
};
