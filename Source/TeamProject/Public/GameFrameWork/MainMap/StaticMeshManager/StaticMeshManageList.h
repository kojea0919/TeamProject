#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StaticMeshManageList.generated.h"

UENUM(BlueprintType)
enum class EStaticMeshType : uint8
{
	Desk,
	Chair,
	BlackBoard,
	Shelf,
	Cabinet,
	Sink,
	ClassroomDoor,
	Tree,
	Dumpster,
	Book,
	GoalPost,
	Bag
};

UCLASS()
class TEAMPROJECT_API UStaticMeshManageList : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<EStaticMeshType, UStaticMesh*> StaticMeshManageMap; 
};
