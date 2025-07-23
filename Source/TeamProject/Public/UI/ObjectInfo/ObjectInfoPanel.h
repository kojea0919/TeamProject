// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Map/Object/Actor/BaseObject.h"
#include "ObjectInfoPanel.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UObjectInfoPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void InitObjectInfoPanel();

	FOnCurrentInteractedObjectChanged OnCurrentInteractedObjectChanged;

	UFUNCTION()
	void UpdateObjectInfo(ABaseObject* Object);

	UFUNCTION()
	void UpdateUI();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "UI Setting")
	class UVerticalBox* W_ObjectInfoPanelVerticalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI Setting")
	TSubclassOf<UUserWidget> ObjectInfoSectionClass;
};
