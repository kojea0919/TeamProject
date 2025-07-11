// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/WaterGun/BaseWaterGun.h"

ABaseWaterGun::ABaseWaterGun()
{	
	bReplicates = true;
	
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	WaterGunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("WaterGunMeshComponent");
	WaterGunMeshComponent->SetupAttachment(Root);
	WaterGunMeshComponent->SetSimulatePhysics(false);

	NozzleLocation = CreateDefaultSubobject<USceneComponent>("NozzleLocation");
	NozzleLocation->SetupAttachment(Root);
		
	ShootAngleLocation = CreateDefaultSubobject<USceneComponent>("ShootAngleLocation");
	ShootAngleLocation->SetupAttachment(Root);
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
