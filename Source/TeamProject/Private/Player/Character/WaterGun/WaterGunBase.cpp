// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/WaterGun/WaterGunBase.h"

// Sets default values
AWaterGunBase::AWaterGunBase()
{
 	PrimaryActorTick.bCanEverTick = false;

	WaterGunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterGunMesh"));
	RootComponent = WaterGunMesh;
	

}


