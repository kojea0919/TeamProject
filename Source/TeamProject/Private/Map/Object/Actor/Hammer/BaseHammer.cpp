// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/Hammer/BaseHammer.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
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
	CollisionBox = CreateDefaultSubobject<UStaticMeshComponent>("CollisionBox");
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->SetupAttachment(Root);
	CollisionBox->SetHiddenInGame(true);

	ObjectTypeTag = STGamePlayTags::Object_Actor_Hammer;
}

void ABaseHammer::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionBox && HasAuthority())
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseHammer::ABaseHammer::OnOverlapBegin);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ABaseHammer::ABaseHammer::OnOverlapEnd);
	}

	SetCollision(true);
}

void ABaseHammer::SetCollision(bool bIsActive)
{
	if (bIsActive)
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseHammer::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnHammerHit(OtherActor);
}

void ABaseHammer::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	OnHammerHitEnd(OtherActor);
}


void ABaseHammer::OnHammerHit_Implementation(AActor* HitActor)
{
	if (!HasAuthority())
		return;
	
	if (!OverlappedActors.Contains(HitActor))
	{
		OverlappedActors.Add(HitActor);
		
		Multicast_ApplyCollision(HitActor);
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

void ABaseHammer::Multicast_ApplyCollision_Implementation(AActor* HitActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("OnHammerCollisionMulticast"));
	UAbilitySystemComponent* AbilitySystemComponent = USTFunctionLibrary::NativeGetParentAbilitySystemComponentFromActor(HitActor);
	if (AbilitySystemComponent != nullptr)
	{
		FGameplayEventData EventData;
		EventData.Instigator = this;
		EventData.Target = HitActor;
		EventData.EventTag = STGamePlayTags::Event_OnHammerHit;

		FGameplayAbilityTargetDataHandle TargetDataHandle;
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitActor);
		TargetDataHandle.Add(TargetData);
			
		AbilitySystemComponent->HandleGameplayEvent(STGamePlayTags::Event_OnHammerHit, &EventData);
	}
}
