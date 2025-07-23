#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFrameWork/MainMap/StaticMeshManager/StaticMeshManageList.h"
#include "StaticMeshManager.generated.h"

UCLASS()
class TEAMPROJECT_API AStaticMeshManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AStaticMeshManager();

public:
	FStaticMeshInfo GetStaticMesh(EStaticMeshType Type) const { return StaticMeshManageList->StaticMeshManageMap[Type]; }
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshManageList> StaticMeshManageList;
	
};
