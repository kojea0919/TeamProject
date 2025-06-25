#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
#include "EffectObjectPool/BaseEffectActor.h"
#include "EffectObjectPool/TestParticleEffectActor.h"

void UEffectObjectPoolSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (UWorld * World = GetWorld())
	{
		World->OnWorldBeginPlay.AddUObject(this ,&UEffectObjectPoolSubSystem::Init);
	}
}

void UEffectObjectPoolSubSystem::Init()
{
	//Array Count Init
	//-------------------------------------
	//int32 Size = InitObjectClassTypeArr.Num();
	//for (int Idx = 0; Idx < Size; ++Idx)
	//{
		//FInitEffectActorInfo Info = InitObjectClassTypeArr[Idx];
		//InitEffect(Info.EffectActorClass, Info.InitSize);
	//}
	//-------------------------------------
}

ABaseEffectActor* UEffectObjectPoolSubSystem::GetEffectObject(TSubclassOf<ABaseEffectActor> EffectObjectClass)
{
	if (EffectPoolMap.Num() == 0)
		return nullptr;

	FEffectActorArr * ResultArr = EffectPoolMap.Find(EffectObjectClass);
	if ( nullptr == ResultArr )
		return nullptr;

	ABaseEffectActor * ReturnObject = GetEffectObject(ResultArr->Arr);
	return ReturnObject;
}

void UEffectObjectPoolSubSystem::ReturnEffectObject(TSubclassOf<ABaseEffectActor> EffectObjectClass,
	ABaseEffectActor* EffectObject)
{	
	if (EffectPoolMap.Num() == 0 || EffectObject == nullptr)
		return;

	FEffectActorArr * ResultArr = EffectPoolMap.Find(EffectObjectClass);
	if ( nullptr == ResultArr )
		return;

	EffectObject->SetEffectEnable(false);
	ResultArr->Arr.Add(EffectObject);
}

void UEffectObjectPoolSubSystem::InitEffect(TSubclassOf<ABaseEffectActor> EffectActorClass, int32 InitSize)
{
	UWorld * World=  GetWorld();
	checkf(World,TEXT("UEffectObjectPoolSubSystem InitArr World Invalid"));
	
	TArray<ABaseEffectActor*> Arr;
	
	//Init Arr InitSize
	//------------------------------------
	for (uint8 Idx = 0 ; Idx < InitSize; ++Idx)
	{
		ABaseEffectActor * Obj = World->SpawnActor<ABaseEffectActor>(EffectActorClass);
		if (nullptr == Obj)
		{
			UE_LOG(LogTemp,Warning,TEXT("UEffectObjectPoolSubSystem InitArr SpawnActor Null"));
			return;
		}

		Obj->SetEffectEnable(false);
		Arr.Add(Obj);
	}	
	//------------------------------------

	FEffectActorArr NewArr;
	NewArr.Arr = Arr;
	EffectPoolMap.Add(EffectActorClass, NewArr);
}

ABaseEffectActor* UEffectObjectPoolSubSystem::GetEffectObject(TArray<ABaseEffectActor*>& Arr) const
{
	if (nullptr == GetWorld())
		return nullptr;

	ABaseEffectActor * Obj;
	//배열이 비어 있으면 새로운 객체 생성
	if (0 == Arr.Num())
	{
		Obj = GetWorld()->SpawnActor<ABaseEffectActor>(ABaseEffectActor::StaticClass());
	}
	else
	{
		Obj = Arr.Pop();
		if (nullptr == Obj)
			return nullptr;
	}

	Obj->SetEffectEnable(true);

	return Obj;
}
