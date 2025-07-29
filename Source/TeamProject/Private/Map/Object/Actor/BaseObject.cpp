// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/BaseObject.h"

#include "NaniteSceneProxy.h"
#include "EffectObjectPool/BaseEffectActor.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Map/Object/AbilitySystem/BaseObjectGameplayAbility.h"
#include "Map/Object/AbilitySystem/ObjectAbilitySystemComponent.h"
#include "Player/Character/RunnerCharacter.h"
#include "Player/Character/Component/Interactive/RunnerInterActiveComponent.h"
#include "UI/MainHUD/PlayerMainHUD.h"
#include "UI/ObjectInfo/ObjectInfoPanel.h"

FOnCurrentInteractedObjectChanged ABaseObject::OnCurrentInteractedObjectChanged;

// Sets default values
ABaseObject::ABaseObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ObjectAbilitySystemComponent = CreateDefaultSubobject<UObjectAbilitySystemComponent>(TEXT("ObjectAbilitySystemComponent"));
}

void ABaseObject::BeginPlay()
{
	Super::BeginPlay();
	
	if (ObjectData.SelfAbilities.Num() > 0)
	{
		if (HasAuthority())
			InitObjectAbility();
	}

	if (HasAuthority())
	{
		if (AMainMapGameMode* GameModeRef = Cast<AMainMapGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameModeRef->OnGameStart.AddUObject(this, &ABaseObject::InitializeObject);
			GameModeRef->OnGameEnd.AddUObject(this, &ABaseObject::ResetObject);
		}
	}
	
	OnCurrentInteractedObjectChanged.AddUObject(this, &ABaseObject::SetOutline);
}

void ABaseObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseObject::SetGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles)
{
	GrantedAbilitySpecHandles = SpecHandles;
}

TArray<FGameplayAbilitySpecHandle> ABaseObject::GetGrantedAbilitySpecHandles() const 
{
	return GrantedAbilitySpecHandles;
}

void ABaseObject::SetOutline(ABaseObject* Object)
{
	if (!Object)
	{
		SetOutlineEnabled(false);
		return;
	}

	if (!Object->Interactable)
		return;
	
	if (Object == this)
	{
		SetOutlineEnabled(true);
	}
	else
	{
		SetOutlineEnabled(false);
	}
}

void ABaseObject::SetOutlineEnabled(bool bIsEnabled)
{
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	GetComponents<UStaticMeshComponent>(StaticMeshComponents);

	for (UStaticMeshComponent* StaticMeshComponent : StaticMeshComponents)
	{
		if (StaticMeshComponent->bRenderCustomDepth == bIsEnabled)
			continue;
		
		StaticMeshComponent->SetRenderCustomDepth(bIsEnabled);
		StaticMeshComponent->SetCustomDepthStencilValue(1);
	}
}

UPawnInterActiveComponent* ABaseObject::GetInterActiveComponent() const
{
	return RunnerInterActiveComponent;
}

void ABaseObject::InitObjectAbility()
{
	if (ObjectAbilitySystemComponent != nullptr)
		ObjectAbilitySystemComponent->AddAbilityToObject(ObjectData.SelfAbilities);
}

void ABaseObject::SetEffectActorTransform(ABaseEffectActor* EffectActor, FTransform Transform)
{
	if (EffectActor == nullptr)
		return;

	EffectActor->SetActorTransform(Transform);
}

void ABaseObject::Server_SetInteractable_Implementation(bool bIsInteractable)
{
	Multicast_SetInteractable(bIsInteractable);
}

void ABaseObject::Multicast_SetInteractable_Implementation(bool bIsInteractable)
{
	Interactable = bIsInteractable;
}

void ABaseObject::UpdateUI()
{
	// 1. World 체크
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 2. PlayerController 체크
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	// 3. Cast 체크
	AMainMapPlayerController* PC = Cast<AMainMapPlayerController>(PlayerController);
	if (!PC)
	{
		return;
	}

	// 4. PlayerMainHUD 체크 (한 번만 호출하고 저장)
	UPlayerMainHUD* MainHUD = PC->GetPlayerMainHUD();
	if (!MainHUD)
	{
		return;
	}

	// 5. ObjectInfoPanel 체크 (한 번만 호출하고 저장)
	UObjectInfoPanel* InfoPanel = MainHUD->GetObjectInfoPanel();
	if (!InfoPanel)
	{
		return;
	}

	// 6. 안전하게 호출
	InfoPanel->UpdateUI();
}

void ABaseObject::GetEffectObjectFromPool_Implementation(ABaseCharacter* Player, TSubclassOf<ABaseEffectActor> EffectActorClass,
                                                         const FTransform& Transform)
{
	UEffectObjectPoolSubSystem* ObjectPool = GetWorld()->GetSubsystem<UEffectObjectPoolSubSystem>();

	ABaseEffectActor* SpawnedEffect = ObjectPool->GetEffectObject(EffectActorClass);

	SetEffectActorTransform(SpawnedEffect, Transform);
	SpawnedEffect->EffectSetUp(Player, this);
}

