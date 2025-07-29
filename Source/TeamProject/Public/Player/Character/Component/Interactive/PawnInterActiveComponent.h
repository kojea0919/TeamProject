// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Character/Component/PawnExtensionComponent.h"
#include "GameplayTagContainer.h"
#include "PawnInterActiveComponent.generated.h"

class ABaseObject;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UPawnInterActiveComponent : public UPawnExtensionComponent
{
	GENERATED_BODY()

public:

	// 생성된 오브젝트 등록
	UFUNCTION(BlueprintCallable, Category = "InterActieve")
	void RegisterSpawnObject(FGameplayTag ObjectTag, ABaseObject* Object, bool bRegisterAsInterActedObject = false);

	// 테그로 상호작용하는 오브젝트 가져오기
	//UFUNCTION(BlueprintCallable, Category = "InterActieve")
	//ABaseObject* GetSpawnObjectByTag(FGameplayTag ObjectTag) const;

	// 캐릭터가 상호작용한 오브젝트의 태그
	//UPROPERTY(BlueprintReadWrite, Category = "InterActieve")
	//FGameplayTag CharacterCurrentInterActedObjectTag;

	UPROPERTY(BlueprintReadWrite, Category = "InterActieve")
	ABaseObject* CharacterEquippedWeapon;

	UPROPERTY(BlueprintReadWrite, Category = "InterActieve")
	bool bEquippedWeaponIsActive = false;

	// 캐릭터가 상호작용한 오브젝트
	UFUNCTION(BlueprintCallable, Category = "InterActieve")
	ABaseObject* GetCharacterEquippedWeapon() const;

	// 캐릭터가 상호작용한 오브젝트
	//UFUNCTION(BlueprintCallable, Category = "InterActieve")
	//ABaseObject* GetCharacterCurrentInterActedObject() const;

	UFUNCTION(BlueprintCallable, Category = "InterActieve")
	ABaseObject* GetCharacterInteractingObject() const;



	TMap<FGameplayTag, ABaseObject*> InteractingObjectsMap;
	
protected:
	TArray<AActor*> InteractingActors;

	UFUNCTION()
	void BroadcastChangeCurrentInteractedObject();

	UPROPERTY()
	TWeakObjectPtr<ABaseObject> CurrentInteractingObject;
	
};
