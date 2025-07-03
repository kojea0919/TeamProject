// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/Object/Subsystem/WorldSubsystem/SpawnerManagerSubsystem.h"
#include "Map/Object/BaseObjectSpawner.h"
#include "Map/Object/Definition/DataAsset/ObjectSpawnData.h"
#include "Map/Object/Actor/BaseObject.h"
#include "Engine/Engine.h"

const FString USpawnerManagerSubsystem::DefaultSpawnDataPath = TEXT("/Game/Temporary/SJS/DT_SpawnData.DT_SpawnData");

void USpawnerManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (GetWorld() && GetWorld()->GetNetMode() != NM_Client)
    {
        RegisteredSpawners.Empty();
        CachedObjectMappings.Empty();
        LoadDefaultSpawnConfiguration();
    }
}

void USpawnerManagerSubsystem::Deinitialize()
{
    if (GetWorld() && GetWorld()->GetNetMode() != NM_Client)
    {
        ClearAllSpawners();
        RegisteredSpawners.Empty();
        CachedObjectMappings.Empty();
        SpawnConfiguration = nullptr;
    }
        
    Super::Deinitialize();
}

void USpawnerManagerSubsystem::RegisterSpawner(ABaseObjectSpawner* Spawner)
{
    if (!GetWorld() || GetWorld()->GetNetMode() == NM_Client || !IsValid(Spawner))
    {
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
    if (!GetWorld() || GetWorld()->GetNetMode() == NM_Client || !IsValid(Spawner))
    {
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
    if (!GetWorld() || GetWorld()->GetNetMode() == NM_Client)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: Spawn execution blocked on client"));
        return;
    }
    
    if (!ValidateSpawnConfiguration())
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: Invalid spawn configuration"));
        return;
    }

    if (RegisteredSpawners.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: No spawners registered"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Starting hybrid spawn execution"));

    // 1. 스폰 요청을 개별 태그로 확장
    TArray<FGameplayTag> TagsToAssign = ExpandSpawnRequests();
    if (TagsToAssign.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: No tags to assign"));
        return;
    }

    // 2. 실행 가능성 사전 체크
    if (!IsAssignmentPossible(TagsToAssign))
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: Assignment impossible - not enough compatible spawners"));
        return;
    }

    // 3. 하이브리드 할당 시도
    TMap<ABaseObjectSpawner*, FGameplayTag> Assignment;
    bool bSuccess = false;

    if (TagsToAssign.Num() <= 15) // 작은 문제
    {
        UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Using backtracking for small problem (size: %d)"), TagsToAssign.Num());
        bSuccess = BacktrackAssignment(TagsToAssign, Assignment);
    }
    else // 큰 문제
    {
        UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Using greedy restart for large problem (size: %d)"), TagsToAssign.Num());
        
        const int32 MaxAttempts = CalculateMaxAttempts(TagsToAssign);
        for (int32 i = 0; i < MaxAttempts && !bSuccess; ++i)
        {
            bSuccess = TryGreedyTagAssignment(TagsToAssign, Assignment);
        }
        
        // 탐욕이 실패하면 백트래킹으로 최후 시도
        if (!bSuccess)
        {
            UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: Greedy failed after %d attempts, trying backtrack fallback"), MaxAttempts);
            bSuccess = BacktrackAssignment(TagsToAssign, Assignment);
        }
    }

    // 4. 결과 처리
    if (bSuccess)
    {
        ExecuteTagAssignment(Assignment);
        UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Spawn execution completed successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: All assignment methods failed!"));
    }
}

TArray<FGameplayTag> USpawnerManagerSubsystem::ExpandSpawnRequests() const
{
    TArray<FGameplayTag> IndividualTags;
    
    if (!IsValid(SpawnConfiguration))
    {
        return IndividualTags;
    }
    
    for (const FSpawnRequest& Request : SpawnConfiguration->SpawnRequests)
    {
        for (int32 i = 0; i < Request.Quantity; ++i)
        {
            IndividualTags.Add(Request.ObjectTypeTag);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Expanded %d spawn requests to %d individual tags"), 
           SpawnConfiguration->SpawnRequests.Num(), IndividualTags.Num());
    
    return IndividualTags;
}

bool USpawnerManagerSubsystem::IsAssignmentPossible(const TArray<FGameplayTag>& TagsToAssign) const
{
    TMap<FGameplayTag, int32> TagCounts;
    for (const FGameplayTag& Tag : TagsToAssign)
    {
        TagCounts.FindOrAdd(Tag)++;
    }
    
    for (const auto& TagCountPair : TagCounts)
    {
        FGameplayTag Tag = TagCountPair.Key;
        int32 RequiredCount = TagCountPair.Value;
        
        int32 AvailableSpawners = 0;
        for (ABaseObjectSpawner* Spawner : RegisteredSpawners)
        {
            if (IsValid(Spawner) && Spawner->GetSpawnTypes().HasTag(Tag))
            {
                AvailableSpawners++;
            }
        }
        
        if (AvailableSpawners < RequiredCount)
        {
            UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: Not enough spawners for tag: %s (need %d, have %d)"), 
                   *Tag.ToString(), RequiredCount, AvailableSpawners);
            return false;
        }
    }
    
    return true;
}

bool USpawnerManagerSubsystem::TryGreedyTagAssignment(
    const TArray<FGameplayTag>& TagsToAssign, 
    TMap<ABaseObjectSpawner*, FGameplayTag>& OutAssignment) const
{
    TArray<FGameplayTag> RemainingTags = TagsToAssign;
    TArray<ABaseObjectSpawner*> AvailableSpawners = RegisteredSpawners;
    
    // 태그 순서 랜덤화
    RemainingTags.Sort([](const FGameplayTag& A, const FGameplayTag& B) {
        return FMath::RandBool();
    });
    
    OutAssignment.Empty();
    
    for (const FGameplayTag& Tag : RemainingTags)
    {
        // 이 태그를 처리할 수 있는 스포너들 찾기
        TArray<ABaseObjectSpawner*> Candidates;
        for (ABaseObjectSpawner* Spawner : AvailableSpawners)
        {
            if (IsValid(Spawner) && Spawner->GetSpawnTypes().HasTag(Tag))
            {
                Candidates.Add(Spawner);
            }
        }
        
        if (Candidates.IsEmpty())
        {
            return false; // 실패
        }
        
        // 스포너 선택 (혼합 전략: 80% 휴리스틱, 20% 랜덤)
        ABaseObjectSpawner* ChosenSpawner = SelectSpawnerForTag(Candidates, RemainingTags, AvailableSpawners);
        
        // 할당하고 사용된 스포너 제거
        OutAssignment.Add(ChosenSpawner, Tag);
        AvailableSpawners.Remove(ChosenSpawner);
    }
    
    return true;
}

bool USpawnerManagerSubsystem::BacktrackAssignment(
    const TArray<FGameplayTag>& TagsToAssign,
    TMap<ABaseObjectSpawner*, FGameplayTag>& OutAssignment,
    int32 TagIndex) const
{
    if (TagIndex >= TagsToAssign.Num())
    {
        return true; // 모든 태그 할당 완료
    }
    
    FGameplayTag CurrentTag = TagsToAssign[TagIndex];
    TArray<ABaseObjectSpawner*> Candidates = GetUnusedSpawnersForTag(CurrentTag, OutAssignment);
    
    // 후보 순서 랜덤화
    Candidates.Sort([](const ABaseObjectSpawner& A, const ABaseObjectSpawner& B) {
        return FMath::RandBool();
    });
    
    for (ABaseObjectSpawner* Spawner : Candidates)
    {
        // 시도
        OutAssignment.Add(Spawner, CurrentTag);
        
        // 재귀적으로 다음 태그 처리
        if (BacktrackAssignment(TagsToAssign, OutAssignment, TagIndex + 1))
        {
            return true; // 성공
        }
        
        // 실패시 롤백
        OutAssignment.Remove(Spawner);
    }
    
    return false;
}

void USpawnerManagerSubsystem::ExecuteTagAssignment(const TMap<ABaseObjectSpawner*, FGameplayTag>& Assignment)
{
    ClearAllSpawners();
    
    for (const auto& AssignmentPair : Assignment)
    {
        ABaseObjectSpawner* Spawner = AssignmentPair.Key;
        FGameplayTag AssignedTag = AssignmentPair.Value;
        
        if (IsValid(Spawner))
        {
            // SpawnManager에서 태그에 따라 랜덤하게 클래스 선택
            TSubclassOf<ABaseObject> SelectedClass = SelectRandomClassForTag(AssignedTag);
            
            if (IsValid(SelectedClass))
            {
                // 스포너에게 구체적인 클래스 전달
                if (Spawner->SpawnObjectClass(SelectedClass))
                {
                    UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Successfully spawned tag %s as class %s"), 
                           *AssignedTag.ToString(), *SelectedClass->GetName());
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: Failed to spawn class %s for tag %s"), 
                           *SelectedClass->GetName(), *AssignedTag.ToString());
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: No valid class found for tag: %s"), *AssignedTag.ToString());
            }
        }
    }
}

ABaseObjectSpawner* USpawnerManagerSubsystem::SelectSpawnerForTag(
    const TArray<ABaseObjectSpawner*>& Candidates,
    const TArray<FGameplayTag>& RemainingTags,
    const TArray<ABaseObjectSpawner*>& AvailableSpawners) const
{
    if (Candidates.Num() == 1)
    {
        return Candidates[0];
    }
    
    int32 RandomIndex = FMath::RandRange(0, Candidates.Num() - 1);
    return Candidates[RandomIndex];
}

TSubclassOf<ABaseObject> USpawnerManagerSubsystem::SelectRandomClassForTag(FGameplayTag ObjectTypeTag) const
{
    TArray<TSubclassOf<ABaseObject>> AvailableClasses = GetAvailableClassesForType(ObjectTypeTag);
    
    if (AvailableClasses.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: No available classes for tag: %s"), *ObjectTypeTag.ToString());
        return nullptr;
    }

    // 랜덤하게 하나 선택
    int32 RandomIndex = FMath::RandRange(0, AvailableClasses.Num() - 1);
    TSubclassOf<ABaseObject> SelectedClass = AvailableClasses[RandomIndex];
    
    UE_LOG(LogTemp, Log, TEXT("SpawnerManagerSubsystem: Selected class %s for tag %s (choice %d of %d)"), 
           *SelectedClass->GetName(), *ObjectTypeTag.ToString(), RandomIndex + 1, AvailableClasses.Num());
    
    return SelectedClass;
}

TArray<ABaseObjectSpawner*> USpawnerManagerSubsystem::GetUnusedSpawnersForTag(
    FGameplayTag Tag,
    const TMap<ABaseObjectSpawner*, FGameplayTag>& CurrentAssignment) const
{
    TArray<ABaseObjectSpawner*> UnusedSpawners;
    
    for (ABaseObjectSpawner* Spawner : RegisteredSpawners)
    {
        if (IsValid(Spawner) && 
            !CurrentAssignment.Contains(Spawner) && 
            Spawner->GetSpawnTypes().HasTag(Tag))
        {
            UnusedSpawners.Add(Spawner);
        }
    }
    
    return UnusedSpawners;
}

int32 USpawnerManagerSubsystem::CalculateMaxAttempts(const TArray<FGameplayTag>& TagsToAssign) const
{
    int32 BaseAttempts = 50;
    float ConstraintDensity = CalculateConstraintDensity(TagsToAssign);
    
    if (ConstraintDensity < 0.3f) // 매우 제약적
    {
        return BaseAttempts * 5;
    }
    else if (ConstraintDensity < 0.6f) // 보통
    {
        return BaseAttempts * 2;
    }
    else // 여유로움
    {
        return BaseAttempts;
    }
}

float USpawnerManagerSubsystem::CalculateConstraintDensity(const TArray<FGameplayTag>& TagsToAssign) const
{
    if (TagsToAssign.IsEmpty() || RegisteredSpawners.IsEmpty())
    {
        return 0.0f;
    }
    
    int32 TotalPossibleConnections = 0;
    int32 ActualConnections = 0;
    
    TSet<FGameplayTag> UniqueTags;
    for (const FGameplayTag& Tag : TagsToAssign)
    {
        UniqueTags.Add(Tag);
    }
    
    for (const FGameplayTag& Tag : UniqueTags)
    {
        for (ABaseObjectSpawner* Spawner : RegisteredSpawners)
        {
            TotalPossibleConnections++;
            if (IsValid(Spawner) && Spawner->GetSpawnTypes().HasTag(Tag))
            {
                ActualConnections++;
            }
        }
    }
    
    return TotalPossibleConnections > 0 ? (float)ActualConnections / TotalPossibleConnections : 0.0f;
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

bool USpawnerManagerSubsystem::ValidateSpawnConfiguration() const
{
    if (!IsValid(SpawnConfiguration))
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnerManagerSubsystem: No spawn configuration loaded"));
        return false;
    }
    
    if (SpawnConfiguration->SpawnRequests.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: No spawn requests in configuration"));
        return false;
    }
    
    if (SpawnConfiguration->ObjectTypeMappings.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnerManagerSubsystem: No object type mappings in configuration"));
        return false;
    }
    
    return true;
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