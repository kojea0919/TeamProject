// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/Actor/BaseObject.h"
#include "BaseHammer.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseHammer : public ABaseObject
{
	GENERATED_BODY()

	ABaseHammer();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* HammerMeshHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* HammerMeshHead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* HammerMeshTop;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* CollisionBox;
};
