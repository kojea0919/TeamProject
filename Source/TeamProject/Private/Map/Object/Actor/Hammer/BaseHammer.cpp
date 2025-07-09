// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/Hammer/BaseHammer.h"

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
	CollisionBox->SetupAttachment(Root);
	CollisionBox->SetHiddenInGame(true);
}
