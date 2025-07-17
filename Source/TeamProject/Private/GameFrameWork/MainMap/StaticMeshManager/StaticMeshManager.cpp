#include "GameFrameWork/MainMap/StaticMeshManager/StaticMeshManager.h"

AStaticMeshManager::AStaticMeshManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AStaticMeshManager::BeginPlay()
{
	Super::BeginPlay();	
}

