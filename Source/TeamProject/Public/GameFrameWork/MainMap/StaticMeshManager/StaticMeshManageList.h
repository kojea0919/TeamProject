#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StaticMeshManageList.generated.h"

UENUM(BlueprintType)
enum class EStaticMeshType : uint8
{
	Desk = 0,
	Chair,
	BlackBoard,
	Shelf,
	Cabinet,
	Sink,
	ClassroomDoor,
	Tree,
	Dumpster,
	WaterDispenser,
	
	None
};

USTRUCT()
struct FStaticMeshInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UStaticMesh * Mesh = nullptr;

	UPROPERTY(EditAnywhere)
	int ZHeight = -5;
};

UCLASS()
class TEAMPROJECT_API UStaticMeshManageList : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<EStaticMeshType, FStaticMeshInfo> StaticMeshManageMap; 
};
