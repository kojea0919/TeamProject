
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EffectObjectPoolSubSystem.generated.h"

class ABaseEffectActor;

USTRUCT()
struct FEffectActorArr
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<ABaseEffectActor*> Arr;
};

UCLASS()
class TEAMPROJECT_API UEffectObjectPoolSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void Init();

public:
	UFUNCTION(BlueprintCallable)
	ABaseEffectActor *GetEffectObject(TSubclassOf<ABaseEffectActor> EffectObjectClass);

	UFUNCTION(BlueprintCallable)
	void ReturnEffectObject(TSubclassOf<ABaseEffectActor> EffectObjectClass, ABaseEffectActor* EffectObject);
	
private:
	// template<typename T>
	// void InitEffect(int32 InitSize);

	void InitEffect(TSubclassOf<ABaseEffectActor> EffectActorClass, int32 InitSize);
	
	ABaseEffectActor* GetEffectObject(TArray<ABaseEffectActor*> & Arr) const; 

protected:
	UPROPERTY()
	TMap<TSubclassOf<ABaseEffectActor>, FEffectActorArr> EffectPoolMap;
	
};

// template<typename T>
// void UEffectObjectPoolSubSystem::InitEffect(int32 InitSize)
// {
// 	UWorld * World=  GetWorld();
// 	checkf(World,TEXT("UEffectObjectPoolSubSystem InitArr World Invalid"));
// 	
// 	TArray<ABaseEffectActor*> Arr;
// 	
// 	//Init Arr InitSize
// 	//------------------------------------
// 	for (uint8 Idx = 0 ; Idx < InitSize; ++Idx)
// 	{
// 		T * Obj = World()->SpawnActor<T>(T::StaticClass());
// 		if (nullptr == Obj)
// 		{
// 			UE_LOG(LogTemp,Warning,TEXT("UEffectObjectPoolSubSystem InitArr SpawnActor Null"));
// 			return;
// 		}
//
// 		Obj->SetEffectEnable(false);
// 		Arr.Add(Obj);
// 	}	
// 	//------------------------------------
//
// 	FEffectActorArr NewArr;
// 	NewArr.Arr = Arr;
// 	EffectPoolMap.Add(T::StaticClass(), NewArr);
// }