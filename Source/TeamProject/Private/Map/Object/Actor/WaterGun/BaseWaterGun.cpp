// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/WaterGun/BaseWaterGun.h"

#include "GameTag/STGamePlayTags.h"

ABaseWaterGun::ABaseWaterGun()
{	
	bReplicates = true;
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	WaterGunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("WaterGunMeshComponent");
	WaterGunMeshComponent->SetupAttachment(Root);
	WaterGunMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WaterGunMeshComponent->SetSimulatePhysics(false);

	
	NozzleLocation = CreateDefaultSubobject<USceneComponent>("NozzleLocation");
	NozzleLocation->SetupAttachment(Root);

	SetActorEnableCollision(true);
	
	ShootAngleLocation = CreateDefaultSubobject<USceneComponent>("ShootAngleLocation");
	ShootAngleLocation->SetupAttachment(Root);

	ObjectTypeTag = STGamePlayTags::Object_Actor_WaterGun;

	Interactable = true;
}

void ABaseWaterGun::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
		Server_SetCurrentWaterAmount(MaxWaterAmount);
}

FRotator ABaseWaterGun::GetShootAngle()
{
	if (!NozzleLocation || !ShootAngleLocation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Component null"));
		return FRotator::ZeroRotator;
	}

	return FRotator::ZeroRotator;
	//return UKismetMathLibrary::FindLookAtRotation(ShootAngleLocation->GetComponentLocation(), NozzleLocation->GetComponentLocation());
}



FText ABaseWaterGun::GetObjectName()
{
	return FText::FromString(TEXT("Water Gun"));
}

FText ABaseWaterGun::GetDescription()
{
	return FText::FromString(FString::Printf(TEXT("Water Amount : %d / %d"), CurrentWaterAmount, MaxWaterAmount));
}

FText ABaseWaterGun::GetStatusText()
{
	return FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentWaterAmount, MaxWaterAmount));
}

void ABaseWaterGun::Server_SetCurrentWaterAmount_Implementation(int Amount)
{
	if (!HasAuthority())
		return;
	Multicast_SetCurrentWaterAmount(Amount);
}

void ABaseWaterGun::Multicast_SetCurrentWaterAmount_Implementation(int Amount)
{
	CurrentWaterAmount = Amount;

	UpdateUI();
}
