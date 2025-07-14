#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "SplashTagGameInstance.generated.h"

UCLASS()
class TEAMPROJECT_API USplashTagGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable)
	void SetIsPlayedMovie() { PlayedMovie = true; };

	UFUNCTION(BlueprintCallable)
	bool IsPlayedMovie() const { return PlayedMovie; };

	void NetworkFailure(UWorld * World, UNetDriver * NetDriver, ENetworkFailure::Type FailureType,const FString & ErrorString);
	
private:
	bool PlayedMovie = false;
};
