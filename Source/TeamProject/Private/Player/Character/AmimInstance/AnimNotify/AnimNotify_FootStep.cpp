// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AmimInstance/AnimNotify/AnimNotify_FootStep.h"

#include "Player/Character/BaseCharacter.h"

void UAnimNotify_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !Sound) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(Owner);
	if (!Character) return;

	if (Character->HasAuthority())
	{
		const FVector Location = MeshComp->GetSocketLocation(SocketName);
		Character->Multicast_PlayerFootStep(Sound,Location, VolumeMultiplier, PitchMultiplier);
		Character->CallFootStepEffect(Location);
	}

	
}
