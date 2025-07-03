// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/Component/PawnExtensionComponent.h"
#include "GameplayTagContainer.h"
#include "RepelComponent.generated.h"

class ABaseWaterGun;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API URepelComponent : public UPawnExtensionComponent
{
	GENERATED_BODY()

public:
	// 물총 등록
	UFUNCTION(BlueprintCallable, Category = "Runner")
	void RegisterSpawnedWaterGun(FGameplayTag WaterGunTag, ABaseWaterGun* WaterGun, bool bRegisterAsEquippedWeapon = false );

	// 캐릭터가 휴대하는 물총 
	UFUNCTION(BlueprintCallable, Category = "Runner")
	ABaseWaterGun* GetCharacterCarriedWaterGunByTag(FGameplayTag WaterGunTag) const;

	// 캐릭터가 장착중인 물총 태그
	UPROPERTY(BlueprintReadWrite, Category = "Runner")
	FGameplayTag CurrentEquippedWaterGunTag;

	// 캐릭터가 장착한 물총
	UFUNCTION(BlueprintCallable, Category = "Runner")
	ABaseWaterGun* GetCharacterCurrentEquippedWaterGun() const;

protected:
	TArray<AActor> OverlappedActors;

private:
	TMap<FGameplayTag, ABaseWaterGun*> RunnerCarriedWaterGunMap;
	
};
