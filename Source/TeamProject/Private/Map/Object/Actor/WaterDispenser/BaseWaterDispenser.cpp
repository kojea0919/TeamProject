// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/WaterDispenser/BaseWaterDispenser.h"

#include "GameTag/STGamePlayTags.h"

ABaseWaterDispenser::ABaseWaterDispenser()
{
	ObjectTypeTag = STGamePlayTags::Object_Actor_WaterDispenser;
	
	if (HasAuthority())
		CurrentWaterAmount = 10;
		//CurrentWaterAmount = FMath::RandRange(0, 10);

	
}

void ABaseWaterDispenser::SetCurrentWaterAmount_Implementation(int Amount)
{
	CurrentWaterAmount = Amount;
}
