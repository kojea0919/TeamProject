// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "InterActiveInterface.generated.h"

class ABaseObject;
class UPawnInterActiveComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterActiveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEAMPROJECT_API IInterActiveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual UPawnInterActiveComponent* GetInterActiveComponent() const = 0;

	// ObjectTag 가져오기
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactive")
	FGameplayTag GetObjectTag() const;


	// Object Class 가져오기
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactive")
	ABaseObject* GetObject() const;

	// 반응 구현 UI 생성등
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactive")
	void Interact(AActor* Interactor);
};
