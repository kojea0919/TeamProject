// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/Hammer/BaseHammer.h"

#include <memory>

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Components/BoxComponent.h"
#include "GameTag/STGamePlayTags.h"
#include "Player/Character/Libraries/STFunctionLibrary.h"

ABaseHammer::ABaseHammer()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	HammerMeshHandle = CreateDefaultSubobject<UStaticMeshComponent>("HammerMeshHandle");
	HammerMeshHandle->SetupAttachment(Root);
	HammerMeshHead = CreateDefaultSubobject<UStaticMeshComponent>("HammerMeshHead");
	HammerMeshHead->SetupAttachment(HammerMeshHandle);
	HammerMeshTop = CreateDefaultSubobject<UStaticMeshComponent>("HammerMeshTop");
	HammerMeshTop->SetupAttachment(HammerMeshHandle);

	HammerMeshHandle->SetWorldScale3D(FVector(0.13f));
	// CollisionBox = CreateDefaultSubobject<UStaticMeshComponent>("CollisionBox");
	// CollisionBox->SetGenerateOverlapEvents(true);
	// CollisionBox->SetupAttachment(Root);
	// CollisionBox->SetHiddenInGame(true);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(RootComponent);
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ObjectTypeTag = STGamePlayTags::Object_Actor_Hammer;

	Interactable = true;
	bReplicates = true;
}

void ABaseHammer::BeginPlay()
{
	Super::BeginPlay();


	if (WeaponCollisionBox && HasAuthority())
	{
		WeaponCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseHammer::ABaseHammer::OnOverlapBegin);
		WeaponCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ABaseHammer::ABaseHammer::OnOverlapEnd);
	}

	//SetCollision(true);
}

void ABaseHammer::SetCollision(bool bIsActive)
{
	if (bIsActive)
		WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseHammer::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnHammerHit(OtherActor, SweepResult);
}

void ABaseHammer::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	OnHammerHitEnd(OtherActor);
}


void ABaseHammer::OnHammerHit_Implementation(AActor* HitActor, const FHitResult& HitResult)
{
	if (!HasAuthority())
		return;

	if (HitActor == nullptr)
		return;
	
	if (!OverlappedActors.Contains(HitActor))
	{
		OverlappedActors.Add(HitActor);
		
		Multicast_ApplyCollision(HitActor, HitResult);
	}
}

void ABaseHammer::OnHammerHitEnd_Implementation(AActor* HitActor)
{
	if (!HasAuthority())
		return;

	if (OverlappedActors.Contains(HitActor))
	{
		OverlappedActors.Remove(HitActor);
	}
}

void ABaseHammer::Multicast_ApplyCollision_Implementation(AActor* HitActor, const FHitResult& HitResult)
{
	if (!HasAuthority())
		return;
	
	UAbilitySystemComponent* AbilitySystemComponent = USTFunctionLibrary::NativeGetParentAbilitySystemComponentFromActor(HitActor);
	if (AbilitySystemComponent != nullptr)
	{
		FGameplayEventData EventData;
		EventData.Instigator = this;
		EventData.Target = HitActor;
		EventData.EventTag = STGamePlayTags::Event_OnHammerHit;

		FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
    
		EventData.TargetData = TargetDataHandle;
		AbilitySystemComponent->HandleGameplayEvent(STGamePlayTags::Event_OnHammerHit, &EventData);
		
	}
}
