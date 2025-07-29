// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FootStep.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UAnimNotify_FootStep : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	FName SocketName;

	UPROPERTY(EditAnywhere, Category = "Sound")
	float PitchMultiplier;
	
	UPROPERTY(EditAnyWhere, Category = "Sound")
	float VolumeMultiplier;
	
};
