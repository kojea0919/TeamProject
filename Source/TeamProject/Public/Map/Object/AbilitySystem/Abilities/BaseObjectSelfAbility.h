// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/AbilitySystem/BaseObjectGameplayAbility.h"
#include "BaseObjectSelfAbility.generated.h"

class ABaseObject;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBaseObjectSelfAbility : public UBaseObjectGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure, Category="Ability")
	ABaseObject* GetObjectFromActorInfo();

	//UFUNCTION(BlueprintPure, Category="Ability")
	//AMainMapPlayerController* GetCharacterControllerFromActorInfo();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ABaseObject> CachedObject;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//TWeakObjectPtr<AMainMapPlayerController> CachedCharacterController;
};
