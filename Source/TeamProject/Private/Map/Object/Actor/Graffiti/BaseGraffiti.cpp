// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/Graffiti/BaseGraffiti.h"

#include "EffectObject/NiagaraEffect/BaseWaterGunBeamEffectActor.h"

ABaseGraffiti::ABaseGraffiti()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	GraffitiMesh = CreateDefaultSubobject<UStaticMeshComponent>("GraffitiMesh");
	GraffitiMesh->SetupAttachment(Root);
}

void ABaseGraffiti::BeginPlay()
{
	Super::BeginPlay();

	ABaseWaterGunBeamEffectActor::OnSplashHit.AddDynamic(this, &ABaseGraffiti::OnHitBySplash);
}

void ABaseGraffiti::OnHitBySplash(AActor* HitActor)
{
	if (HasAuthority())
		Server_Request_OnSplashHit();
}

void ABaseGraffiti::Server_Request_OnSplashHit_Implementation()
{
	
}

void ABaseGraffiti::Apply_OnSplashHit_Implementation()
{
	
}
