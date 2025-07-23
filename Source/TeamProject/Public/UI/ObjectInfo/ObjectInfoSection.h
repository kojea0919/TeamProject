// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectInfoSection.generated.h"

class UTextBlock;
class ABaseObject;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UObjectInfoSection : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void InitObjectInfo(ABaseObject* Object);

	UFUNCTION()
	void UpdateUI();

	UFUNCTION()
	void SetObjectName();

	UFUNCTION()
	void SetDescription();

	UFUNCTION()
	ABaseObject* GetCachedObject() const { return Cached_Object.Get(); }

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> ObjectNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> DescriptionTextBlock;

private:
	TWeakObjectPtr<ABaseObject> Cached_Object;
};
