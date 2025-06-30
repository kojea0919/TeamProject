// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameTag/STGamePlayTags.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpawnerManagerSubsystem.generated.h"

class ABaseObject;
class UObjectSpawnData;
class ABaseObjectSpawner;

struct FSpawnRequest;
struct FObjectClassMapping;


/**
 * 월드 내 모든 오브젝트 스포너를 관리하고 스폰 요청을 처리하는 서브시스템
 * 스폰 데이터 에셋을 기반으로 오브젝트들을 적절한 스포너에 할당하여 생성
 */
UCLASS()
class TEAMPROJECT_API USpawnerManagerSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // UWorldSubsystem interface
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // 스포너 등록/해제
    UFUNCTION(BlueprintCallable, Category = "Spawner Management")
    void RegisterSpawner(ABaseObjectSpawner* Spawner);

    UFUNCTION(BlueprintCallable, Category = "Spawner Management")
    void UnregisterSpawner(ABaseObjectSpawner* Spawner);

    // 스폰 실행
    UFUNCTION(BlueprintCallable, Category = "Spawn Execution")
    void ExecuteSpawnRequests();

    // 스포너 정보 조회
    UFUNCTION(BlueprintCallable, Category = "Spawner Query")
    FGameplayTagContainer GetSpawnerSupportedTypes(ABaseObjectSpawner* Spawner) const;

    UFUNCTION(BlueprintCallable, Category = "Spawner Query")
    int32 GetRegisteredSpawnerCount() const { return RegisteredSpawners.Num(); }

    UFUNCTION(BlueprintCallable, Category = "Spawner Query")
    TArray<ABaseObjectSpawner*> GetSpawnersByType(FGameplayTag ObjectTypeTag) const;

    // 스폰 데이터 관리
    UFUNCTION(BlueprintCallable, Category = "Configuration")
    bool LoadSpawnConfiguration(const FString& DataAssetPath);

    UFUNCTION(BlueprintCallable, Category = "Configuration")
    void ClearAllSpawners();

protected:
    // 등록된 모든 스포너들
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawners")
    TArray<ABaseObjectSpawner*> RegisteredSpawners;

    // 스폰 설정 데이터 에셋
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configuration")
    TObjectPtr<UObjectSpawnData> SpawnConfiguration;

private:
    // 캐시된 오브젝트 타입 매핑 정보
    UPROPERTY()
    TArray<FObjectClassMapping> CachedObjectMappings;

    // 내부 헬퍼 함수들
    void LoadDefaultSpawnConfiguration();
    void CacheObjectMappings();
    ABaseObjectSpawner* FindBestSpawnerForType(FGameplayTag ObjectTypeTag) const;
    bool ValidateSpawnConfiguration() const;

    // 기본 스폰 데이터 경로
    static const FString DefaultSpawnDataPath;

    void ProcessSpawnRequest(const FSpawnRequest& SpawnRequest);
    TArray<TSubclassOf<ABaseObject>> GetAvailableClassesForType(FGameplayTag ObjectTypeTag) const;
};