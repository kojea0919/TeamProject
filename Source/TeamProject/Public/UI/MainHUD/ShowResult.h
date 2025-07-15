#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShowResult.generated.h"


UCLASS()
class TEAMPROJECT_API UShowResult : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	void Init();

	void ShowResult(bool IsTaggerWin);

	UFUNCTION()
	void OnFadeOutFinished();
	
protected:
	UPROPERTY(meta=(BindWidgetANim) ,Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUserWidget> W_RunnerWin;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUserWidget> W_TaggerWin;

	FWidgetAnimationDynamicEvent FadeOutEndDelegate;
};
