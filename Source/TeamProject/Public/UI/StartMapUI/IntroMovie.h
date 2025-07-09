#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IntroMovie.generated.h"

UCLASS()
class TEAMPROJECT_API UIntroMovie : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetSquareProgressTime(float Time);

	UFUNCTION(BlueprintCallable)
	void InitSquareProgress();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressSquare> WB_ProgressSquare;
	
};
