// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/Component/PawnExtensionComponent.h"
#include "GameplayTagContainer.h"
#include "Map/Object/Actor/BaseWeapon.h"
#include "RepelComponent.generated.h"

class ABaseWeapon;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand,
	
};

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API URepelComponent : public UPawnExtensionComponent
{
	GENERATED_BODY()

public:
	// 무기 등록
	UFUNCTION(BlueprintCallable, Category = "Repel")
	virtual void RegisterSpawnedWeapon(FGameplayTag WeaponTag, ABaseWeapon* Weapon, bool bRegisterAsEquippedWeapon = false );

	// 캐릭터가 휴대하는 무기
	UFUNCTION(BlueprintCallable, Category = "Repel")
	ABaseWeapon* GetCharacterCarriedWeaponByTag(FGameplayTag WeaponTag) const;

	// 캐릭터가 장착중인 무기 태그
	UPROPERTY(BlueprintReadWrite, Category = "Repel")
	FGameplayTag CurrentEquippedWeaponTag;

	// 캐릭터가 장착한 무기
	UFUNCTION(BlueprintCallable, Category = "Repel")
	ABaseWeapon* GetCharacterCurrentEquippedWeapon() const;

private:
	TMap<FGameplayTag, ABaseWeapon*> CarriedWeaponMap;
		
};
