// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Libraries/STAbilitySystemLibrary.h"

#include "GameFrameWork/Test/GameMode/STGameMode.h"
#include "Kismet/GameplayStatics.h"

UCharacterClassInfo* USTAbilitySystemLibrary::GetCharacterClassDefaultInfo(const UObject* WorldContextObject)
{
	if (const ASTGameMode* STGameMode = Cast<ASTGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return STGameMode->GetCharacterClassDefaultInfo();
	}

	return nullptr;
}
