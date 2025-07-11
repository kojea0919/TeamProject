#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressSquare.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UProgressSquare : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetProgress(float Time);

	UFUNCTION(BlueprintCallable)
	void InitProgress();

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Img_ProgressSquare;

	UPROPERTY(EditAnywhere, meta = (allowPrivateAccess = true))
	class UMaterialInterface* ProgressSquareMaterial;
};
