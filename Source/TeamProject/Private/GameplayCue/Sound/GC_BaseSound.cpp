// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCue/Sound/GC_BaseSound.h"

#include "Kismet/GameplayStatics.h"

bool UGC_BaseSound::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	if (Sound && Target)
	{
		UGameplayStatics::PlaySoundAtLocation(Target, Sound, Target->GetActorLocation());
		return true;
	}
	return false;
}
