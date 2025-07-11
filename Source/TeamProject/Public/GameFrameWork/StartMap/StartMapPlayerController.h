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

	UFUNCTION(BlueprintCallable)
	void EndMovie();
	
private:
	void InitHUD();
	void InitInputMode();

private:
	//StartMap HUD
	//---------------------------------------------------------------------
	UPROPERTY(EditAnywhere, Category = HUD, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UStartMapHUD> StartMapHUDClass;
	
	UPROPERTY()
	class UStartMapHUD* StartMapHUD;
	//---------------------------------------------------------------------

	//IntroMovie
	//---------------------------------------------------------------------
	UPROPERTY(EditAnywhere,Category= HUD,meta=(AllowPrivateAccess = "true"))
	TSubclassOf<class UIntroMovie> IntroMovieClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UIntroMovie* IntroMovieWidget;
	//---------------------------------------------------------------------
};
