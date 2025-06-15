#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMapHUD.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UStartMapHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void ClickCreateSessionButton();

	UFUNCTION()
	void ClickJoinSessionButton();

private:
	//UPROPERTY()
	//class UButton* CreateSessionButton;

	//UPROPERTY()
	//class UButton* JoinSessionButton;
};
