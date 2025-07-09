// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ObjectSpawnRequestData.generated.h"

class ABaseObject;

USTRUCT(BlueprintType)
struct FSpawnRequest
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="Object"))
	FGameplayTag ObjectTypeTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="1"))
	int32 Quantity = 1;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRandomizeSelection = true;  // 랜덤 선택 여부
};

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UObjectSpawnRequestData : public UDataAsset
{
	GENERATED_BODY()

public:
	// 실제 스폰할 오브젝트들과 수량 정의
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Requests")
	TArray<FSpawnRequest> SpawnRequests;

	UFUNCTION(BlueprintCallable, Category = "Spawn Request Data")
	bool AddRequestData(const FSpawnRequest& SpawnRequest);

	UFUNCTION(BlueprintCallable, Category = "Spawn Request Data")
	bool RemoveRequestData(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Spawn Request Data")
	void Clear();
};