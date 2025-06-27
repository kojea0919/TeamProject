// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/Object/Subsystem/WorldSubsystem/SpawnerManagerSubsystem.h"
#include "Map/Object/BaseObjectSpawner.h"
#include "Map/Object/Actor/Door/Door01.h"
#include "Map/Object/Definition/DataAsset/ObjectSpawnData.h"
#include "Engine/Engine.h"

// 기본 스폰 데이터 경로 정의
const FString USpawnerManagerSubsystem::DefaultSpawnDataPath = TEXT("/Game/Temporary/SJS/DT_SpawnData.DT_SpawnData");

void USpawnerManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // 배열 초기화
    RegisteredSpawners.Empty();
    CachedObjectMappings.Empty();
    
    // 기본 스폰 설정 로드
    LoadDefaultSpawnConfiguration();
    
    UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: Initialized successfully"));
}

void USpawnerManagerSubsystem::Deinitialize()
{
    // 모든 스포너 정리
    ClearAllSpawners();
    
    // 배열 정리
    RegisteredSpawners.Empty();
    CachedObjectMappings.Empty();
    
    // 스폰 설정 해제
    SpawnConfiguration = nullptr;
    
    Super::Deinitialize();
    
    UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: Deinitialized"));
}

void USpawnerManagerSubsystem::RegisterSpawner(ABaseObjectSpawner* Spawner)
{
    if (!IsValid(Spawner))
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: Attempted to register invalid spawner"));
        return;
    }

    if (RegisteredSpawners.Contains(Spawner))
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: Spawner already registered"));
        return;
    }

    RegisteredSpawners.Add(Spawner);
    UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Spawner registered successfully. Total: %d"), RegisteredSpawners.Num());
}

void USpawnerManagerSubsystem::UnregisterSpawner(ABaseObjectSpawner* Spawner)
{
    if (!IsValid(Spawner))
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: Attempted to unregister invalid spawner"));
        return;
    }

    if (RegisteredSpawners.Contains(Spawner))
    {
        RegisteredSpawners.Remove(Spawner);
        UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Spawner unregistered. Remaining: %d"), RegisteredSpawners.Num());
    }
}

void USpawnerManagerSubsystem::ExecuteSpawnRequests()
{
    if (!IsValid(SpawnConfiguration))
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: No spawn configuration available"));
        return;
    }

    if (RegisteredSpawners.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: No spawners registered"));
        return;
    }

    // 모든 스포너 클리어
    ClearAllSpawners();
    
    UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Starting spawn execution"));

    // 스폰 요청 처리
    for (const FSpawnRequest& SpawnRequest : SpawnConfiguration->SpawnRequests)
    {
        ProcessSpawnRequest(SpawnRequest);
    }
    
    UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Spawn execution completed"));
}

FGameplayTagContainer USpawnerManagerSubsystem::GetSpawnerSupportedTypes(ABaseObjectSpawner* Spawner) const
{
    if (!IsValid(Spawner))
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: Attempted to get types from invalid spawner"));
        return FGameplayTagContainer();
    }

    return Spawner->GetSpawnTypes();
}

TArray<ABaseObjectSpawner*> USpawnerManagerSubsystem::GetSpawnersByType(FGameplayTag ObjectTypeTag) const
{
    TArray<ABaseObjectSpawner*> MatchingSpawners;
    
    for (ABaseObjectSpawner* Spawner : RegisteredSpawners)
    {
        if (IsValid(Spawner))
        {
            FGameplayTagContainer SupportedTypes = Spawner->GetSpawnTypes();
            if (SupportedTypes.HasTag(ObjectTypeTag))
            {
                MatchingSpawners.Add(Spawner);
            }
        }
    }
    
    return MatchingSpawners;
}

bool USpawnerManagerSubsystem::LoadSpawnConfiguration(const FString& DataAssetPath)
{
    UObjectSpawnData* NewSpawnData = LoadObject<UObjectSpawnData>(nullptr, *DataAssetPath);
    
    if (!IsValid(NewSpawnData))
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: Failed to load spawn configuration from path: %s"), *DataAssetPath);
        return false;
    }
    
    SpawnConfiguration = NewSpawnData;
    CacheObjectMappings();
    
    UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Spawn configuration loaded successfully from: %s"), *DataAssetPath);
    return true;
}

void USpawnerManagerSubsystem::ClearAllSpawners()
{
    for (ABaseObjectSpawner* Spawner : RegisteredSpawners)
    {
        if (IsValid(Spawner))
        {
            Spawner->Clear();
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: All spawners cleared"));
}

void USpawnerManagerSubsystem::LoadDefaultSpawnConfiguration()
{
    if (!LoadSpawnConfiguration(DefaultSpawnDataPath))
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: Failed to load default spawn configuration"));
    }
}

void USpawnerManagerSubsystem::CacheObjectMappings()
{
    CachedObjectMappings.Empty();
    
    if (IsValid(SpawnConfiguration))
    {
        CachedObjectMappings = SpawnConfiguration->ObjectTypeMappings;
        UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Cached %d object mappings"), CachedObjectMappings.Num());
    }
}

ABaseObjectSpawner* USpawnerManagerSubsystem::FindBestSpawnerForType(FGameplayTag ObjectTypeTag) const
{
    TArray<ABaseObjectSpawner*> CompatibleSpawners = GetSpawnersByType(ObjectTypeTag);
    
    if (CompatibleSpawners.IsEmpty())
    {
        return nullptr;
    }
    
    // 첫 번째 호환 가능한 스포너 반환 (추후 더 정교한 로직 추가 가능)
    return CompatibleSpawners[0];
}

bool USpawnerManagerSubsystem::ValidateSpawnConfiguration() const
{
    if (!IsValid(SpawnConfiguration))
    {
        return false;
    }
    
    // 스폰 요청이 있는지 확인
    if (SpawnConfiguration->SpawnRequests.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: No spawn requests in configuration"));
        return false;
    }
    
    // 오브젝트 매핑이 있는지 확인
    if (SpawnConfiguration->ObjectTypeMappings.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: No object type mappings in configuration"));
        return false;
    }
    
    return true;
}

void USpawnerManagerSubsystem::ProcessSpawnRequest(const FSpawnRequest& SpawnRequest)
{
    // 해당 타입을 처리할 수 있는 스포너 찾기
    ABaseObjectSpawner* TargetSpawner = FindBestSpawnerForType(SpawnRequest.ObjectTypeTag);
    
    if (!IsValid(TargetSpawner))
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: No suitable spawner found for type: %s"), 
               *SpawnRequest.ObjectTypeTag.ToString());
        return;
    }
    
    // 해당 타입에 대한 스폰 가능한 클래스들 찾기
    TArray<TSubclassOf<ABaseObject>> AvailableClasses = GetAvailableClassesForType(SpawnRequest.ObjectTypeTag);
    
    if (AvailableClasses.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: No available classes for type: %s"), 
               *SpawnRequest.ObjectTypeTag.ToString());
        return;
    }
    
    // 스폰 실행
    for (int32 i = 0; i < SpawnRequest.Quantity; ++i)
    {
        TSubclassOf<ABaseObject> ClassToSpawn;
        
        if (SpawnRequest.bRandomizeSelection)
        {
            // 랜덤하게 클래스 선택
            int32 RandomIndex = FMath::RandRange(0, AvailableClasses.Num() - 1);
            ClassToSpawn = AvailableClasses[RandomIndex];
        }
        else
        {
            // 첫 번째 클래스 사용
            ClassToSpawn = AvailableClasses[0];
        }
        
        if (IsValid(ClassToSpawn))
        {
            TargetSpawner->SpawnObjectClass(ClassToSpawn);
            UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Spawned object of type: %s"), 
                   *SpawnRequest.ObjectTypeTag.ToString());
        }
    }
}

TArray<TSubclassOf<ABaseObject>> USpawnerManagerSubsystem::GetAvailableClassesForType(FGameplayTag ObjectTypeTag) const
{
    for (const FObjectClassMapping& Mapping : CachedObjectMappings)
    {
        if (Mapping.ObjectTypeTag == ObjectTypeTag)
        {
            return Mapping.AvailableClasses;
        }
    }
    
    return TArray<TSubclassOf<ABaseObject>>();
}