// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/PlayerState/STPlayerState.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"
#include "Player/Character/AbilitySystem/Attributes/STAttributeSet.h"
#include "Net/UnrealNetwork.h"

ASTPlayerState::ASTPlayerState()
{
	NetUpdateFrequency = 100.f;
	MinNetUpdateFrequency = 66.f;

	STAbilitySystemComponent = CreateDefaultSubobject<USTAbilitySystemComponent>("AbilitySystemComp");
	STAbilitySystemComponent->SetIsReplicated(true);
	STAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	STAttributeSet = CreateDefaultSubobject<USTAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* ASTPlayerState::GetAbilitySystemComponent() const
{
	return STAbilitySystemComponent;
}

USTAbilitySystemComponent* ASTPlayerState::GetSTAbilitySystemComponent() const
{
	return STAbilitySystemComponent;
}

USTAttributeSet* ASTPlayerState::GetSTAttributeSet() const
{
	return STAttributeSet;
}

void ASTPlayerState::InitState()
{
	IsTagger = false;
}

void ASTPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASTPlayerState,ServerNumberID);
	DOREPLIFETIME(ASTPlayerState,PlayerNickName);
}
