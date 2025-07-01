#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FindSessionsCallbackProxy.h"
#include "StartMapPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AStartMapPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;	

	UFUNCTION(BlueprintCallable)
	void AddToSessionList(FBlueprintSessionResult SessionResult);

	UFUNCTION(BlueprintCallable)
	void ClearSessionList();

	UFUNCTION(BlueprintCallable)
	void SetVisibleSessionLoadImage(bool Visible);
	
private:
	void InitHUD();
	void InitInputMode();

private:
	UPROPERTY(EditAnywhere, Category = HUD, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UStartMapHUD> StartMapHUDClass;

	UPROPERTY()
	class UStartMapHUD* StartMapHUD;

};
