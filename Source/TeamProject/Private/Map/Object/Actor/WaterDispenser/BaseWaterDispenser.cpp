// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/WaterDispenser/BaseWaterDispenser.h"

#include "GameTag/STGamePlayTags.h"

ABaseWaterDispenser::ABaseWaterDispenser()
{
	ObjectTypeTag = STGamePlayTags::Object_Actor_WaterDispenser;
}

void ABaseWaterDispenser::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
		SetCurrentWaterAmount(10);
}

void ABaseWaterDispenser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		FColor::Green,
		FString::Printf(TEXT("Dispenser Current Water Amount : %d"), GetCurrentWaterAmount())
	);
}

void ABaseWaterDispenser::SetCurrentWaterAmount_Implementation(int Amount)
{
	CurrentWaterAmount = Amount;
}