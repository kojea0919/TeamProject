// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FindSessionsCallbackProxy.h"
#include "SessionListBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API USessionListBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void AddSessionList(FBlueprintSessionResult SessionResult);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UVerticalBox * VB_SessionListbox;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class USessionList> SessionListClass;
};
