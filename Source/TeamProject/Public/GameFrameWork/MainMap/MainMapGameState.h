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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE EGameState GetCurrentGameState() const { return CurGameState;}	
private:
	UFUNCTION()
	void UpdateSecond();
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_RemainSecond)
	int RemainSecond;

	FTimerHandle SecondUpdateTimerHandle;

	UPROPERTY(Replicated)
	EGameState CurGameState;
};
