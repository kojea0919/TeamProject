#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrisonSpawnLocation.generated.h"

UCLASS()
class TEAMPROJECT_API APrisonSpawnLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	APrisonSpawnLocation();

protected:
	virtual void BeginPlay() override;
};
