// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Map/Object/Definition/Struct/ObjectStruct.h"
#include "GameplayTagContainer.h"
#include "Player/Character/Interface/InterActiveInterface.h"
#include "BaseObject.generated.h"

class URunnerInterActiveComponent;
struct FGameplayAbilitySpecHandle;
struct FObjectStruct;

UCLASS()
class TEAMPROJECT_API ABaseObject : public AActor, public IInterActiveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseObject();
	

	UPROPERTY(EditAnywhere, Category = "Custom | ObjectTag")
	FGameplayTag ObjectTypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ObjectData")
	FObjectDataStruct ObjectData;

	void SetGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles);
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;


	// Interface
	virtual FGameplayTag GetObjectTag_Implementation() const override {return ObjectTypeTag;}
	virtual ABaseObject* GetObject_Implementation() const override {return const_cast<ABaseObject*>(this);}

	// InteractiveComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InterActive", meta =(AllowPrivateAccess = true))
	URunnerInterActiveComponent* RunnerInterActiveComponent;
	
	FORCEINLINE URunnerInterActiveComponent* GetRunnerInterActiveComponent() const { return RunnerInterActiveComponent; }

private:

	
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;

protected:
	virtual UPawnInterActiveComponent* GetInterActiveComponent() const override;
};
