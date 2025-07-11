#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "SplashTagGameInstance.generated.h"

UCLASS()
class TEAMPROJECT_API USplashTagGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetIsPlayedMovie() { PlayedMovie = true; };

	UFUNCTION(BlueprintCallable)
	bool IsPlayedMovie() const { return PlayedMovie; };

private:
	bool PlayedMovie = false;
};
