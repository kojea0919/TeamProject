// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameWork/MainMap/MainMapPlayerState.h"

#include "Net/UnrealNetwork.h"

void AMainMapPlayerState::InitState()
{
	IsTagger = false;
}

void AMainMapPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainMapPlayerState,ServerNumberID);
	DOREPLIFETIME(AMainMapPlayerState,PlayerNickName);
}
