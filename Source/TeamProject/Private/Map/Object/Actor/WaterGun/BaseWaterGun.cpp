// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/WaterGun/BaseWaterGun.h"

ABaseWaterGun::ABaseWaterGun()
{
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	WaterGunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("WaterGunMeshComponent");
	WaterGunMeshComponent->SetupAttachment(Root);
	WaterGunMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WaterGunMeshComponent->SetSimulatePhysics(false);

	NozzleLocation = CreateDefaultSubobject<USceneComponent>("NozzleLocation");
	NozzleLocation->SetupAttachment(Root);

	SetActorEnableCollision(false);
	
}
