// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Libraries/STAbilitySystemLibrary.h"

#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "Kismet/GameplayStatics.h"

UCharacterClassInfo* USTAbilitySystemLibrary::GetCharacterClassDefaultInfo(const UObject* WorldContextObject)
{
	if (const AMainMapGameMode* MainMapGameMode = Cast<AMainMapGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return MainMapGameMode->GetCharacterClassDefaultInfo();
	}

	return nullptr;
}
