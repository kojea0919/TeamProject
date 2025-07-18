// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "STPlayerState.generated.h"

class USTAttributeSet;
class USTAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ASTPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()


public:

	ASTPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure)
	USTAbilitySystemComponent* GetSTAbilitySystemComponent() const;
	UFUNCTION(BlueprintPure)
	USTAttributeSet* GetSTAttributeSet() const;


public:
	FORCEINLINE void SetTagger() { IsTagger = true;}
	void InitState();
	FORCEINLINE bool IsPlayerTargger() const { return IsTagger;}
	FORCEINLINE void SetPlayerNickName(const FString & NickName) { PlayerNickName = NickName; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(Replicated)
	int ServerNumberID;

	UPROPERTY(Replicated)
	FString PlayerNickName;
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USTAbilitySystemComponent> STAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USTAttributeSet> STAttributeSet;

	UPROPERTY(Replicated)
	bool IsTagger = false;
};
