// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Object/Actor/BaseObject.h"
#include "BaseGraffiti.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ABaseGraffiti : public ABaseObject
{
	GENERATED_BODY()

	ABaseGraffiti();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* GraffitiMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInterface* BaseMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMaterialInstanceDynamic* DynamicMaterial;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	bool GetIsErased() const;

protected:
	UPROPERTY()
	float EraseRatio = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxRatio = 0.6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EraseSpeed = 0.01f;
	
	UPROPERTY()
	bool bIsErased = false;
	
	UFUNCTION()
	void OnHitBySplash(AActor* HitActor);

	UFUNCTION(Server, Reliable)
	void Server_Request_OnSplashHit(AActor* HitActor);

	UFUNCTION(NetMulticast, Reliable)
	void Apply_OnSplashHit(float NewEraseRatio);
};
