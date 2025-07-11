#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
#include "EffectObjectPool/BaseEffectActor.h"
#include "EffectObjectPool/TestParticleEffectActor.h"
#include "EffectObjectPool/EfffectObjectPoolInitInfo.h"

UEffectObjectPoolSubSystem::UEffectObjectPoolSubSystem()
{
	static ConstructorHelpers::FObjectFinder<UDataAsset> DA_InitInfo(TEXT("/Game/_GamePlay/EffectObjectPool/InitObjectListDataAsset/DA_EffectObjPoolInitInfo.DA_EffectObjPoolInitInfo"));
	if (DA_InitInfo.Succeeded())
	{
		InitInfo = Cast<UEfffectObjectPoolInitInfo>(DA_InitInfo.Object);
	}
}

void UEffectObjectPoolSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);	

	//GameInstance를 확인해서 플레이 맵이 아닌 경우에는 Init안하기

	
	if (UWorld * World = GetWorld())
	{		
		World->OnWorldBeginPlay.AddUObject(this ,&UEffectObjectPoolSubSystem::Init);
	}
}

void UEffectObjectPoolSubSystem::Init()
{
	//Array Count Init
	//-------------------------------------
	checkf(InitInfo, TEXT("UEffectObjectPoolSubSystem::Init() InitInfo Null"));
	
	int8 Size = InitInfo->InitInfoArr.Num();
	for (int8 Idx = 0 ; Idx < Size; ++Idx)
	{
		InitEffect(InitInfo->InitInfoArr[Idx].EffectActorClass,InitInfo->InitInfoArr[Idx].InitSize);
	}	
	//-------------------------------------
}

ABaseEffectActor* UEffectObjectPoolSubSystem::GetEffectObject(TSubclassOf<ABaseEffectActor> EffectObjectClass)
{
	if (EffectPoolMap.Num() == 0)
		return nullptr;

	FEffectActorArr * ResultArr = EffectPoolMap.Find(EffectObjectClass);
	if ( nullptr == ResultArr )
		return nullptr;


	ABaseEffectActor * ReturnObject = GetEffectObject(ResultArr->Arr, EffectObjectClass);
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
	ResultArr->Arr.Add(
		EffectObject);
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

ABaseEffectActor* UEffectObjectPoolSubSystem::GetEffectObject(TArray<ABaseEffectActor*>& Arr, TSubclassOf<ABaseEffectActor> EffectObjectClass) const
{
	if (nullptr == GetWorld())
		return nullptr;
	
	ABaseEffectActor * Obj;
	//배열이 비어 있으면 새로운 객체 생성
	if (0 == Arr.Num())
	{
		Obj = GetWorld()->SpawnActor<ABaseEffectActor>(EffectObjectClass);
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
