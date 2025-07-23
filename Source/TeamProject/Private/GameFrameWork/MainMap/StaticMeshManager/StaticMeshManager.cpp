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
}

