// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/Graffiti/BaseGraffiti.h"

#include "VisualizeTexture.h"
#include "EffectObject/NiagaraEffect/BaseWaterGunBeamEffectActor.h"

ABaseGraffiti::ABaseGraffiti()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	GraffitiMesh = CreateDefaultSubobject<UStaticMeshComponent>("GraffitiMesh");
	GraffitiMesh->SetupAttachment(Root);
}

void ABaseGraffiti::BeginPlay()
{
	Super::BeginPlay();

	ABaseWaterGunBeamEffectActor::OnSplashHit.AddDynamic(this, &ABaseGraffiti::OnHitBySplash);

	if (BaseMaterial && GraffitiMesh)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		GraffitiMesh->SetMaterial(0, DynamicMaterial);

		DynamicMaterial->SetScalarParameterValue(FName("Opacity"), MaxRatio);
	}
}

void ABaseGraffiti::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(FName("Opacity"), MaxRatio - EraseRatio);
	}
}

bool ABaseGraffiti::GetIsErased() const
{
	return bIsErased;
}

void ABaseGraffiti::OnHitBySplash(AActor* HitActor)
{
	if (HasAuthority())
		Server_Request_OnSplashHit(HitActor);
}

void ABaseGraffiti::Server_Request_OnSplashHit_Implementation(AActor* HitActor)
{
	if (HitActor == this)
	{
		EraseRatio += EraseSpeed;

		if (EraseRatio >= MaxRatio)
			EraseRatio = MaxRatio;
	
		Apply_OnSplashHit(EraseRatio);
	}
}

void ABaseGraffiti::Apply_OnSplashHit_Implementation(float NewEraseRatio)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("OnSplashHit"));
	EraseRatio = NewEraseRatio;
}
