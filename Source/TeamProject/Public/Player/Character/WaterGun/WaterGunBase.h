// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterGunBase.generated.h"

UCLASS()
class TEAMPROJECT_API AWaterGunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterGunBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaterGun")
	UStaticMeshComponent* WaterGunMesh;

	

};
