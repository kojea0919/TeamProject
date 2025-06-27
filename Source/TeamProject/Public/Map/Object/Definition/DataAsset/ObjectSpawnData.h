// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ObjectSpawnData.generated.h"

class ABaseObject;

USTRUCT(BlueprintType)
struct FObjectClassMapping
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="Object"))
	FGameplayTag ObjectTypeTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ABaseObject>> AvailableClasses;
};

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
class TEAMPROJECT_API UObjectSpawnData : public UDataAsset
{
	GENERATED_BODY()

public:
	// 각 오브젝트 타입별로 스폰 가능한 클래스들 정의
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	TArray<FObjectClassMapping> ObjectTypeMappings;

	// 실제 스폰할 오브젝트들과 수량 정의
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Requests")
	TArray<FSpawnRequest> SpawnRequests;

	// 유틸리티 함수들
	UFUNCTION(BlueprintCallable, Category = "Spawn Data")
	TArray<TSubclassOf<ABaseObject>> GetAvailableClasses(FGameplayTag ObjectTypeTag) const;
    
	UFUNCTION(BlueprintCallable, Category = "Spawn Data")
	TSubclassOf<ABaseObject> GetRandomClass(FGameplayTag ObjectTypeTag) const;
};