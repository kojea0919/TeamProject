#include "GameFrameWork/MainMap/StaticMeshManager/StaticMeshManager.h"

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

