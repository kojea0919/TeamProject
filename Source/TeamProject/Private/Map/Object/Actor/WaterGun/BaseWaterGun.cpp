// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/WaterGun/BaseWaterGun.h"

ABaseWaterGun::ABaseWaterGun()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	WaterGunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("WaterGunMeshComponent");
	WaterGunMeshComponent->SetupAttachment(Root);

	NozzleLocation = CreateDefaultSubobject<USceneComponent>("NozzleLocation");
	NozzleLocation->SetupAttachment(Root);
}
