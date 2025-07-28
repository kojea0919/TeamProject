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
	{
		if (AMainMapGameMode* GameModeRef = Cast<AMainMapGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameModeRef->OnGameStart.AddUObject(this, &ABaseWaterDispenser::InitializeObject);
		}
	}
		
}

void ABaseWaterDispenser::SetMaxWaterAmount_Implementation(int Amount)
{
	MaxWaterAmount = Amount;

	if (HasAuthority())
		OnMaxAmountChange();
}

void ABaseWaterDispenser::OnCurrentAmountChange()
{
	UpdateUI();
}

void ABaseWaterDispenser::OnMaxAmountChange()
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

void ABaseWaterDispenser::InitializeObject(EGameMode GameMode)
{
	if (!HasAuthority())
		return;
	
	int randomNumber = (rand() % 26) + 5;
	
	SetMaxWaterAmount(randomNumber);
	SetCurrentWaterAmount(randomNumber);
}

void ABaseWaterDispenser::SetCurrentWaterAmount_Implementation(int Amount)
{
	CurrentWaterAmount = Amount;

	if (HasAuthority())
		OnCurrentAmountChange();
}
