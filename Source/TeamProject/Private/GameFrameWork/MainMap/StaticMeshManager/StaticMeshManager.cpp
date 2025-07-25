#include "GameFrameWork/MainMap/StaticMeshManager/StaticMeshManager.h"
#include "GameFrameWork/MainMap/StaticMeshManager/StaticMeshManageList.h"

AStaticMeshManager::AStaticMeshManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
}

void AStaticMeshManager::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Pair : StaticMeshManageList->StaticMeshManageMap)
	{
		if (Pair.Value.Mesh)
		{
			// 강제로 접근해서 메모리로 로딩
			FString Dummy = Pair.Value.Mesh->GetName();
		}
	}
}

