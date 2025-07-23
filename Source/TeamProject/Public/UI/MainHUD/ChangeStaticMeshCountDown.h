#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChangeStaticMeshCountDown.generated.h"

UCLASS()
class TEAMPROJECT_API UChangeStaticMeshCountDown : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCountDownText(int Second);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Tb_CountDown;
};
