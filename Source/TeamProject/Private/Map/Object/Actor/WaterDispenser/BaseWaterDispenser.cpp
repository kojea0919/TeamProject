// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/WaterDispenser/BaseWaterDispenser.h"

#include "GameTag/STGamePlayTags.h"
#include "Net/UnrealNetwork.h"

ABaseWaterDispenser::ABaseWaterDispenser()
{
	ObjectTypeTag = STGamePlayTags::Object_Actor_WaterDispenser;

	Interactable = true;
}

void ABaseWaterDispenser::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
		SetCurrentWaterAmount(10);
}

void ABaseWaterDispenser::OnCurrentAmountChange()
{
	UpdateUI();
}

FText ABaseWaterDispenser::GetObjectName()
{
	return FText::FromString(TEXT("Water Dispenser"));
}

FText ABaseWaterDispenser::GetDescription()
{
	return FText::FromString(FString::Printf(TEXT("Water Amount : %d / %d"), GetCurrentWaterAmount(), MaxWaterAmount));
}

void ABaseWaterDispenser::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseWaterDispenser, CurrentWaterAmount);
	DOREPLIFETIME(ABaseWaterDispenser, MaxWaterAmount);
}

void ABaseWaterDispenser::SetCurrentWaterAmount_Implementation(int Amount)
{
	CurrentWaterAmount = Amount;

	if (HasAuthority())
		OnCurrentAmountChange();
}
