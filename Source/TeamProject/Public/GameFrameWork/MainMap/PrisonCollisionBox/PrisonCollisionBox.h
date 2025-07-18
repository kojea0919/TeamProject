#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrisonCollisionBox.generated.h"

UCLASS()
class TEAMPROJECT_API APrisonCollisionBox : public AActor
{
	GENERATED_BODY()
	
public:	
	APrisonCollisionBox();

public:
	int Check();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComponent;
};
