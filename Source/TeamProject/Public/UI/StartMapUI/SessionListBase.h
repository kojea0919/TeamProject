// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FindSessionsCallbackProxy.h"
#include "SessionListBase.generated.h"

DECLARE_DELEGATE(FOnFrameExitButtonClicked);

//새로운 주석
UCLASS()
class TEAMPROJECT_API USessionListBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void AddSessionList(FBlueprintSessionResult SessionResult);

	void ClearSessionList();
	void SetVisibleSessionLoadImage(bool bVisible);

private:
	UFUNCTION()
	void ClickExit();

public:
	FOnFrameExitButtonClicked OnFrameExitButtonClicked;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VB_SessionListbox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Exit;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUserWidget> WB_LoadingSession;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<class USessionList> SessionListClass;
};
