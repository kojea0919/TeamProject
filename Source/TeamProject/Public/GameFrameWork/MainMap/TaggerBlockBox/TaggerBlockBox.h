#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaggerBlockBox.generated.h"

UCLASS()
class TEAMPROJECT_API ATaggerBlockBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ATaggerBlockBox();

	UFUNCTION(NetMulticast, reliable)
	void SetBlock(bool BlockOn);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UBoxComponent * BlockBox;
};
