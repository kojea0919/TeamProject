// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AimDot.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UAimDot : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AimDot")
	void SetAimImg(UTexture2D* AimImg);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Aim;
	
};
