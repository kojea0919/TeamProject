#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FindSessionsCallbackProxy.h"
#include "SessionList.generated.h"

UCLASS()
class TEAMPROJECT_API USessionList : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void SetSessionResult(FBlueprintSessionResult TargetResult) { SessionResult = TargetResult; }
	void SetHostName(FString Name);
	void SetSessionPlayerNumber(int8 Number);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* Tb_HostName;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* Tb_SessionPlayerNumber;

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	FBlueprintSessionResult SessionResult;	
};
